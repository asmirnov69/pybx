function(gen_pybx_cpp_stubs out_var)
  set(result)
  foreach(in_f ${ARGN})
    get_filename_component(src_inc_dir ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    get_filename_component(out_f "${in_f}" NAME_WE)
    set(out_f "${out_f}_pybx.h")
    add_custom_command(OUTPUT ${out_f}
      COMMAND ${CMAKE_COMMAND} -E env ${PYBX_HOME}/bin/pybx gen_cpp[fn=${in_f},out_fn=${out_f}]
      DEPENDS ${in_f}
      COMMENT "generating defs for ${in_f} -> ${out_f}"
      VERBATIM
      )
    list(APPEND result ${out_f})
  endforeach()
  set(${out_var} "${result}" PARENT_SCOPE)
endfunction()
