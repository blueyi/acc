if(NOT LLVM_EXTERNAL_LIT)
  message(WARNING "LLVM_EXTERNAL_LIT is not set; skipping lit tests.")
  return()
endif()

set(ACOMPILER_LIT_ARGS "" CACHE STRING "Arguments to pass to lit")

configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.py
  MAIN_CONFIG
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.cfg.py
)

add_lit_testsuite(check-acompiler "Running ACompiler regression tests"
  ${CMAKE_CURRENT_BINARY_DIR}
  ARGS ${ACOMPILER_LIT_ARGS}
  DEPENDS
    acompiler-opt
)

