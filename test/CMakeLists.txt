configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.py
  MAIN_CONFIG
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.cfg.py
)

set(ACOMPILER_TEST_DEPENDS
  ac-opt
  ac-translate
)

add_lit_testsuite(check-acompiler "Running the ACompiler regression tests"
  ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${ACOMPILER_TEST_DEPENDS}
)
set_target_properties(check-acompiler PROPERTIES FOLDER "Tests")

add_lit_testsuites(ACOMPILER ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${ACOMPILER_TEST_DEPENDS})
