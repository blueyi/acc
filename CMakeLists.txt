cmake_minimum_required(VERSION 3.20)

project(ACompiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(ACOMPILER_ENABLE_LIT_TESTS "Enable lit-based tests" ON)

# Standalone LLVM/MLIR project layout:
# The user is expected to provide LLVM_DIR / MLIR_DIR pointing to the
# corresponding CMake config directories.
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}" "${MLIR_CMAKE_DIR}")
include(TableGen)
include(AddLLVM)
include(AddMLIR)
include(HandleLLVMOptions)

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS} ${MLIR_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_definitions(${LLVM_DEFINITIONS})

set(ACOMPILER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ACOMPILER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(lib)
add_subdirectory(tools)

if(ACOMPILER_ENABLE_LIT_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

