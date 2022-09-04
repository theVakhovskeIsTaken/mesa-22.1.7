/*
 * Copyright (C) 2020 Collabora Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "compiler.h"
#include "bi_builder.h"

/* Not all 8-bit and 16-bit instructions support all swizzles on all sources.
 * These passes, intended to run after NIR->BIR but before scheduling/RA, lower
 * away swizzles that cannot be represented. In the future, we should try to
 * recombine swizzles where we can as an optimization.
 */

static void
bi_lower_swizzle_16(bi_context *ctx, bi_instr *ins, unsigned src)
{
        /* Identity is ok */
        if (ins->src[src].swizzle == BI_SWIZZLE_H01)
                return;

        /* TODO: Use the opcode table and be a lot more methodical about this... */
        switch (ins->op) {
        /* Some instructions used with 16-bit data never have swizzles */
        case BI_OPCODE_CSEL_V2F16:
        case BI_OPCODE_CSEL_V2I16:
        case BI_OPCODE_CSEL_V2S16:
        case BI_OPCODE_CSEL_V2U16:

        /* Despite ostensibly being 32-bit instructions, CLPER does not
         * inherently interpret the data, so it can be used for v2f16
         * derivatives, which might require swizzle lowering */
        case BI_OPCODE_CLPER_I32:
        case BI_OPCODE_CLPER_V6_I32:

        /* Similarly, CSEL.i32 consumes a boolean as a 32-bit argument. If the
         * boolean is implemented as a 16-bit integer, the swizzle is needed
         * for correct operation if the instruction producing the 16-bit
         * boolean does not replicate to both halves of the containing 32-bit
         * register. As such, we may need to lower a swizzle.
         *
         * This is a silly hack. Ideally, code gen would be smart enough to
         * avoid this case (by replicating). In practice, silly hardware design
         * decisions force our hand here.
         */
        case BI_OPCODE_MUX_I32:
        case BI_OPCODE_CSEL_I32:
            break;

        case BI_OPCODE_IADD_V2S16:
        case BI_OPCODE_IADD_V2U16:
        case BI_OPCODE_ISUB_V2S16:
        case BI_OPCODE_ISUB_V2U16:
            if (src == 0 && ins->src[src].swizzle != BI_SWIZZLE_H10)
                    break;
            else
                    return;
        case BI_OPCODE_LSHIFT_AND_V2I16:
        case BI_OPCODE_LSHIFT_OR_V2I16:
        case BI_OPCODE_LSHIFT_XOR_V2I16:
        case BI_OPCODE_RSHIFT_AND_V2I16:
        case BI_OPCODE_RSHIFT_OR_V2I16:
        case BI_OPCODE_RSHIFT_XOR_V2I16:
            if (src == 2)
                    return;
            else
                    break;

        /* For some reason MUX.v2i16 allows swaps but not replication */
        case BI_OPCODE_MUX_V2I16:
                if (ins->src[src].swizzle == BI_SWIZZLE_H10)
                        return;
                else
                        break;

        /* We don't want to deal with reswizzling logic in modifier prop. Move
         * the swizzle outside, it's easier for clamp propagation. */
        case BI_OPCODE_FCLAMP_V2F16:
        {
                bi_builder b = bi_init_builder(ctx, bi_after_instr(ins));
                bi_index dest = ins->dest[0];
                bi_index tmp = bi_temp(ctx);

                ins->dest[0] = tmp;
                bi_swz_v2i16_to(&b, dest, bi_replace_index(ins->src[0], tmp));
                return;
        }

        default:
            return;
        }

        /* First, try to apply a given swizzle to a constant to clear the
         * runtime swizzle. This is less heavy-handed than ignoring the
         * swizzle for scalar destinations, since it maintains
         * replication of the destination.
         */
        if (ins->src[src].type == BI_INDEX_CONSTANT) {
                ins->src[src].value = bi_apply_swizzle(ins->src[src].value,
                                                       ins->src[src].swizzle);
                ins->src[src].swizzle = BI_SWIZZLE_H01;
                return;
        }

        /* Even if the source does not replicate, if the consuming instruction
         * produces a 16-bit scalar, we can ignore the other component.
         */
        if (ins->dest[0].swizzle == BI_SWIZZLE_H00 &&
                        ins->src[src].swizzle == BI_SWIZZLE_H00)
        {
                ins->src[src].swizzle = BI_SWIZZLE_H01;
                return;
        }

        /* Lower it away */
        bi_builder b = bi_init_builder(ctx, bi_before_instr(ins));
        ins->src[src] = bi_replace_index(ins->src[src],
                        bi_swz_v2i16(&b, ins->src[src]));
        ins->src[src].swizzle = BI_SWIZZLE_H01;
}

static bool
bi_swizzle_replicates_8(enum bi_swizzle swz)
{
        switch (swz) {
        case BI_SWIZZLE_B0000:
        case BI_SWIZZLE_B1111:
        case BI_SWIZZLE_B2222:
        case BI_SWIZZLE_B3333:
                return true;
        default:
                return false;
        }
}

static bool
bi_swizzle_replicates_16(enum bi_swizzle swz)
{
        switch (swz) {
        case BI_SWIZZLE_H00:
        case BI_SWIZZLE_H11:
                return true;
        default:
                /* If a swizzle replicates every 8-bits, it also replicates
                 * every 16-bits, so allow 8-bit replicating swizzles.
                 */
                return bi_swizzle_replicates_8(swz);
        }
}

static bool
bi_instr_replicates(bi_instr *I, BITSET_WORD *replicates_16)
{
        switch (I->op) {

        /* Instructions that construct vectors have replicated output if their
         * sources are identical. Check this case first.
         */
        case BI_OPCODE_MKVEC_V2I16:
        case BI_OPCODE_V2F16_TO_V2S16:
        case BI_OPCODE_V2F16_TO_V2U16:
        case BI_OPCODE_V2F32_TO_V2F16:
        case BI_OPCODE_V2S16_TO_V2F16:
        case BI_OPCODE_V2S8_TO_V2F16:
        case BI_OPCODE_V2S8_TO_V2S16:
        case BI_OPCODE_V2U16_TO_V2F16:
        case BI_OPCODE_V2U8_TO_V2F16:
        case BI_OPCODE_V2U8_TO_V2U16:
                return bi_is_value_equiv(I->src[0], I->src[1]);

        /* 16-bit transcendentals are defined to output zero in their
         * upper half, so they do not replicate
         */
        case BI_OPCODE_FRCP_F16:
        case BI_OPCODE_FRSQ_F16:
                return false;

        /* Not sure, be conservative, we don't use these.. */
        case BI_OPCODE_VN_ASST1_F16:
        case BI_OPCODE_FPCLASS_F16:
        case BI_OPCODE_FPOW_SC_DET_F16:
                return false;

        default:
                break;
        }

        /* Replication analysis only makes sense for ALU instructions */
        if (bi_opcode_props[I->op].message != BIFROST_MESSAGE_NONE)
                return false;

        /* We only analyze 16-bit instructions for 16-bit replication. We could
         * maybe do better.
         */
        if (bi_opcode_props[I->op].size != BI_SIZE_16)
                return false;

        bi_foreach_src(I, s) {
                if (bi_is_null(I->src[s]))
                        continue;

                /* Replicated swizzles */
                if (bi_swizzle_replicates_16(I->src[s].swizzle))
                        continue;

                /* Replicated values */
                if (bi_is_ssa(I->src[s]) &&
                    BITSET_TEST(replicates_16, bi_word_node(I->src[s])))
                        continue;

                /* Replicated constants */
                if (I->src[s].type == BI_INDEX_CONSTANT &&
                    (I->src[s].value & 0xFFFF) == (I->src[s].value >> 16))
                        continue;

                return false;
        }

        return true;
}

void
bi_lower_swizzle(bi_context *ctx)
{
        bi_foreach_instr_global_safe(ctx, ins) {
                bi_foreach_src(ins, s) {
                        if (!bi_is_null(ins->src[s]))
                                bi_lower_swizzle_16(ctx, ins, s);
                }
        }

        /* Now that we've lowered swizzles, clean up the mess */
        BITSET_WORD *replicates_16 = calloc(sizeof(bi_index), ((ctx->ssa_alloc + 1) << 2));

        bi_foreach_instr_global(ctx, ins) {
                if (bi_is_ssa(ins->dest[0]) && bi_instr_replicates(ins, replicates_16))
                        BITSET_SET(replicates_16, bi_word_node(ins->dest[0]));

                if (ins->op == BI_OPCODE_SWZ_V2I16 && bi_is_ssa(ins->src[0]) &&
                    BITSET_TEST(replicates_16, bi_word_node(ins->src[0]))) {
                        ins->op = BI_OPCODE_MOV_I32;
                        ins->src[0].swizzle = BI_SWIZZLE_H01;
                }

                /* The above passes rely on replicating destinations.  For
                 * Valhall, we will want to optimize this. For now, default
                 * to Bifrost compatible behaviour.
                 */
                ins->dest[0].swizzle = BI_SWIZZLE_H01;
        }

        free(replicates_16);
}