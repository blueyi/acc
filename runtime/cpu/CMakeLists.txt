add_library(ACRuntimeCPU SHARED
  cpu_runtime.cpp
  cpu_kernels.cpp
  memory_manager.cpp
)

target_include_directories(ACRuntimeCPU PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Enable OpenMP for parallel execution
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(ACRuntimeCPU PUBLIC OpenMP::OpenMP_CXX)
endif()
