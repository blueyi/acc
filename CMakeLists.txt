cmake_minimum_required(VERSION 3.20)
project(ACC
  VERSION 0.1.0
  DESCRIPTION "An AI Compiler based on LLVM/MLIR for learning and practice"
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==============================================================================
# Options
# ==============================================================================
option(AC_ENABLE_TESTS "Enable testing" ON)
option(AC_ENABLE_CUDA "Enable NVIDIA CUDA backend (TODO)" OFF)
option(AC_ENABLE_ASCEND "Enable Huawei Ascend NPU backend (TODO)" OFF)

# ==============================================================================
# Find LLVM and MLIR
# ==============================================================================
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Found MLIR in: ${MLIR_DIR}")

list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(TableGen)
include(AddLLVM)
include(AddMLIR)
include(HandleLLVMOptions)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

link_directories(${LLVM_BUILD_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})

# ==============================================================================
# ACC sub-directories
# ==============================================================================
add_subdirectory(include/ACC)
add_subdirectory(lib)
add_subdirectory(runtime)
add_subdirectory(tools)

# ==============================================================================
# Testing
# ==============================================================================
if(AC_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
  add_subdirectory(unittests)
endif()
