# Copyright © 2017 Intel Corporation

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

files_r600 = files(
  'r600d_common.h',
  'compute_memory_pool.c',
  'compute_memory_pool.h',
  'eg_asm.c',
  'eg_debug.c',
  'eg_sq.h',
  'evergreen_compute.c',
  'evergreen_compute.h',
  'evergreen_compute_internal.h',
  'evergreend.h',
  'evergreen_hw_context.c',
  'evergreen_state.c',
  'r600_asm.c',
  'r600_asm.h',
  'r600_blit.c',
  'r600d.h',
  'r600_dump.c',
  'r600_dump.h',
  'r600_formats.h',
  'r600_hw_context.c',
  'r600_isa.c',
  'r600_isa.h',
  'r600_opcodes.h',
  'r600_pipe.c',
  'r600_pipe.h',
  'r600_public.h',
  'r600_shader.c',
  'r600_shader.h',
  'r600_sq.h',
  'r600_state.c',
  'r600_state_common.c',
  'r600_uvd.c',
  'r700_asm.c',
  'r700_sq.h',
  'cayman_msaa.c',
  'r600_buffer_common.c',
  'r600_cs.h',
  'r600_gpu_load.c',
  'r600_perfcounter.c',
  'r600_pipe_common.c',
  'r600_pipe_common.h',
  'r600_query.c',
  'r600_query.h',
  'r600_streamout.c',
  'r600_test_dma.c',
  'r600_texture.c',
  'r600_viewport.c',
  'radeon_uvd.c',
  'radeon_uvd.h',
  'radeon_vce.c',
  'radeon_vce.h',
  'radeon_video.c',
  'radeon_video.h',
  'sb/sb_bc_builder.cpp',
  'sb/sb_bc_decoder.cpp',
  'sb/sb_bc_dump.cpp',
  'sb/sb_bc_finalize.cpp',
  'sb/sb_bc.h',
  'sb/sb_bc_parser.cpp',
  'sb/sb_context.cpp',
  'sb/sb_core.cpp',
  'sb/sb_dce_cleanup.cpp',
  'sb/sb_def_use.cpp',
  'sb/sb_dump.cpp',
  'sb/sb_expr.cpp',
  'sb/sb_expr.h',
  'sb/sb_gcm.cpp',
  'sb/sb_gvn.cpp',
  'sb/sb_if_conversion.cpp',
  'sb/sb_ir.cpp',
  'sb/sb_ir.h',
  'sb/sb_liveness.cpp',
  'sb/sb_pass.cpp',
  'sb/sb_pass.h',
  'sb/sb_peephole.cpp',
  'sb/sb_psi_ops.cpp',
  'sb/sb_public.h',
  'sb/sb_ra_checker.cpp',
  'sb/sb_ra_coalesce.cpp',
  'sb/sb_ra_init.cpp',
  'sb/sb_sched.cpp',
  'sb/sb_sched.h',
  'sb/sb_shader.cpp',
  'sb/sb_shader.h',
  'sb/sb_ssa_builder.cpp',
  'sb/sb_valtable.cpp',
  'sfn/sfn_alu_defines.cpp',
  'sfn/sfn_alu_defines.h',
  'sfn/sfn_callstack.cpp',
  'sfn/sfn_callstack.h',
  'sfn/sfn_conditionaljumptracker.cpp',
  'sfn/sfn_conditionaljumptracker.h',
  'sfn/sfn_defines.h',
  'sfn/sfn_debug.cpp',
  'sfn/sfn_debug.h',
  'sfn/sfn_emitaluinstruction.cpp',
  'sfn/sfn_emitaluinstruction.h',
  'sfn/sfn_emitinstruction.cpp',
  'sfn/sfn_emitinstruction.h',
  'sfn/sfn_emitssboinstruction.cpp',
  'sfn/sfn_emitssboinstruction.h',
  'sfn/sfn_emittexinstruction.cpp',
  'sfn/sfn_emittexinstruction.h',
  'sfn/sfn_emitinstruction.h',
  'sfn/sfn_instruction_alu.cpp',
  'sfn/sfn_instruction_alu.h',
  'sfn/sfn_instruction_base.cpp',
  'sfn/sfn_instruction_base.h',
  'sfn/sfn_instruction_block.cpp',
  'sfn/sfn_instruction_block.h',
  'sfn/sfn_instruction_cf.cpp',
  'sfn/sfn_instruction_cf.h',
  'sfn/sfn_instruction_export.cpp',
  'sfn/sfn_instruction_export.h',
  'sfn/sfn_instruction_fetch.cpp',
  'sfn/sfn_instruction_fetch.h',
  'sfn/sfn_instruction_gds.cpp',
  'sfn/sfn_instruction_gds.h',
  'sfn/sfn_instruction_lds.cpp',
  'sfn/sfn_instruction_lds.h',
  'sfn/sfn_instruction_misc.cpp',
  'sfn/sfn_instruction_misc.h',
  'sfn/sfn_instruction_tex.cpp',
  'sfn/sfn_instruction_tex.h',
  'sfn/sfn_ir_to_assembly.cpp',
  'sfn/sfn_ir_to_assembly.h',
  'sfn/sfn_liverange.cpp',
  'sfn/sfn_liverange.h',
  'sfn/sfn_nir.cpp',
  'sfn/sfn_nir.h',
  'sfn/sfn_nir_legalize_image_load_store.cpp',
  'sfn/sfn_nir_lower_64bit.cpp',
  'sfn/sfn_nir_lower_alu.cpp',
  'sfn/sfn_nir_lower_alu.h',
  'sfn/sfn_nir_lower_fs_out_to_vector.cpp',
  'sfn/sfn_nir_lower_fs_out_to_vector.h',
  'sfn/sfn_nir_lower_tess_io.cpp',
  'sfn/sfn_nir_vectorize_vs_inputs.c',
  'sfn/sfn_shader_base.cpp',
  'sfn/sfn_shader_base.h',
  'sfn/sfn_shader_compute.cpp',
  'sfn/sfn_shader_compute.h',
  'sfn/sfn_shader_fragment.cpp',
  'sfn/sfn_shader_fragment.h',
  'sfn/sfn_shader_geometry.cpp',
  'sfn/sfn_shader_geometry.h',
  'sfn/sfn_shader_tcs.cpp',
  'sfn/sfn_shader_tcs.h',
  'sfn/sfn_shader_tess_eval.cpp',
  'sfn/sfn_shader_tess_eval.h',
  'sfn/sfn_shader_vertex.cpp',
  'sfn/sfn_shader_vertex.h',
  'sfn/sfn_shaderio.cpp',
  'sfn/sfn_shaderio.h',
  'sfn/sfn_value.cpp',
  'sfn/sfn_value.h',
  'sfn/sfn_value_gpr.cpp',
  'sfn/sfn_value_gpr.h',
  'sfn/sfn_valuepool.cpp',
  'sfn/sfn_valuepool.h',
  'sfn/sfn_vertexstageexport.cpp',
  'sfn/sfn_vertexstageexport.h',
  )

egd_tables_h = custom_target(
  'egd_tables.h',
  input : ['egd_tables.py', 'evergreend.h'],
  output : 'egd_tables.h',
  command : [prog_python, '@INPUT@'],
  capture : true,
)

r600_c_args = []
if with_gallium_opencl
  r600_c_args += '-DHAVE_OPENCL'
endif

libr600 = static_library(
  'r600',
  [files_r600, egd_tables_h],
  c_args : [r600_c_args, '-Wstrict-overflow=0'],
  gnu_symbol_visibility : 'hidden',
  include_directories : [
    inc_src, inc_mapi, inc_mesa, inc_include, inc_compiler, inc_gallium, inc_gallium_aux, inc_amd_common,
    inc_gallium_drivers,
  ],
  dependencies: [dep_libdrm_radeon, dep_elf, dep_llvm, idep_nir, idep_nir_headers],
)

driver_r600 = declare_dependency(
  compile_args : '-DGALLIUM_R600',
  link_with : [libr600, libmesa, libradeonwinsys],
)
