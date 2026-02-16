cmake_minimum_required(VERSION 3.20)
project(ACompiler LANGUAGES CXX C)

#-------------------------------------------------------------------------------
# Project Options
#-------------------------------------------------------------------------------

option(ACOMPILER_BUILD_TOOLS "Build ACompiler tools" ON)
option(ACOMPILER_BUILD_RUNTIME "Build ACompiler runtime" ON)
option(ACOMPILER_BUILD_TESTS "Build ACompiler tests" ON)
option(ACOMPILER_BUILD_EXAMPLES "Build ACompiler examples" ON)
option(ACOMPILER_ENABLE_CUDA "Enable CUDA backend" OFF)
option(ACOMPILER_ENABLE_ASCEND "Enable Ascend backend" OFF)

#-------------------------------------------------------------------------------
# C++ Standard
#-------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

#-------------------------------------------------------------------------------
# Build Type
#-------------------------------------------------------------------------------

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#-------------------------------------------------------------------------------
# Output Directories
#-------------------------------------------------------------------------------

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#-------------------------------------------------------------------------------
# Find LLVM and MLIR
#-------------------------------------------------------------------------------

find_package(MLIR REQUIRED CONFIG)

message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/bin)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(TableGen)
include(AddLLVM)
include(AddMLIR)
include(HandleLLVMOptions)

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

#-------------------------------------------------------------------------------
# Compiler Flags
#-------------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wno-unused-parameter)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -march=native)
  endif()
endif()

#-------------------------------------------------------------------------------
# LLVM/MLIR Components
#-------------------------------------------------------------------------------

set(LLVM_LINK_COMPONENTS
  Core
  Support
  )

#-------------------------------------------------------------------------------
# Subdirectories
#-------------------------------------------------------------------------------

add_subdirectory(include/acompiler)
add_subdirectory(lib)

if(ACOMPILER_BUILD_TOOLS)
  add_subdirectory(tools)
endif()

if(ACOMPILER_BUILD_RUNTIME)
  add_subdirectory(runtime)
endif()

if(ACOMPILER_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

if(ACOMPILER_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------

install(DIRECTORY include/acompiler
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  PATTERN "*.td" EXCLUDE
  )

#-------------------------------------------------------------------------------
# Summary
#-------------------------------------------------------------------------------

message(STATUS "")
message(STATUS "ACompiler Configuration Summary:")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  LLVM Version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "  MLIR Version: ${MLIR_VERSION}")
message(STATUS "  Build Tools: ${ACOMPILER_BUILD_TOOLS}")
message(STATUS "  Build Runtime: ${ACOMPILER_BUILD_RUNTIME}")
message(STATUS "  Build Tests: ${ACOMPILER_BUILD_TESTS}")
message(STATUS "  Build Examples: ${ACOMPILER_BUILD_EXAMPLES}")
message(STATUS "  Enable CUDA: ${ACOMPILER_ENABLE_CUDA}")
message(STATUS "  Enable Ascend: ${ACOMPILER_ENABLE_ASCEND}")
message(STATUS "")
