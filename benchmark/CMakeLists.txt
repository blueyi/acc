add_executable(acompiler-bench-matmul cpu/bench_matmul.cpp)
target_link_libraries(acompiler-bench-matmul PRIVATE ACompiler::Core)

add_executable(acompiler-bench-conv cpu/bench_conv.cpp)
target_link_libraries(acompiler-bench-conv PRIVATE ACompiler::Core)
