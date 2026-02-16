add_executable(acompiler-smoke-test smoke/smoke_test.cpp)
target_link_libraries(acompiler-smoke-test PRIVATE ACompiler::Core)
add_test(NAME acompiler_smoke COMMAND acompiler-smoke-test)
