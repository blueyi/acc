# ACompiler - AI Compiler based on LLVM/MLIR
# Minimum CMake: 3.15
cmake_minimum_required(VERSION 3.15)
project(ACompiler
  VERSION 0.1.0
  LANGUAGES CXX
  DESCRIPTION "AI Compiler for learning compiler concepts via LLVM/MLIR"
)

# Export compile commands for IDE
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Load project options
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ACompilerOptions)
include(FindLLVM)
include(FindMLIR)

# Find LLVM (includes MLIR when built together)
# 需指定: -DLLVM_DIR=<llvm_build>/lib/cmake/llvm
#         -DMLIR_DIR=<llvm_build>/lib/cmake/mlir
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Found MLIR")

# Add LLVM/MLIR definitions for all targets
add_definitions(${LLVM_DEFINITIONS})
add_definitions(${MLIR_DEFINITIONS})

# Include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Turn off unused includes warning
add_definitions(-D_GNU_SOURCE)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_LIMIT_MACROS)

# Disable RTTI if LLVM was built without it
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# Subdirectories
add_subdirectory(lib)
add_subdirectory(tools)

# Optional: tests and examples (can be toggled)
option(ACOMPILER_BUILD_TESTS "Build ACompiler tests" ON)
option(ACOMPILER_BUILD_EXAMPLES "Build ACompiler examples" ON)
if(ACOMPILER_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
if(ACOMPILER_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
