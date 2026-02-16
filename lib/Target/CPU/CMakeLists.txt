# CPU 后端
add_library(ACCPUTarget
  CPUPasses.cpp
)
target_include_directories(ACCPUTarget
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ACCPUTarget
  PUBLIC MLIRIR
)
