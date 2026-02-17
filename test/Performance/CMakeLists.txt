# Performance benchmarks
# Build with: ninja bench-matmul bench-conv2d

add_executable(bench-matmul bench_matmul.cpp)
target_link_libraries(bench-matmul PRIVATE ACRuntimeCPU)
target_include_directories(bench-matmul PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bench-conv2d bench_conv2d.cpp)
target_link_libraries(bench-conv2d PRIVATE ACRuntimeCPU)
target_include_directories(bench-conv2d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
