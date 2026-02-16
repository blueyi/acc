cmake_minimum_required(VERSION 3.24)

project(ACompiler VERSION 0.1.0 LANGUAGES C CXX)

include(cmake/ACompilerOptions.cmake)
include(cmake/LLVMConfigHelpers.cmake)

acompiler_set_default_options()

option(ACOMPILER_ENABLE_LLVM "Enable LLVM/MLIR integration" ON)
option(ACOMPILER_BUILD_TOOLS "Build command line tools" ON)
option(ACOMPILER_BUILD_RUNTIME "Build runtime support library" ON)
option(ACOMPILER_BUILD_TESTS "Build tests" ON)
option(ACOMPILER_BUILD_BENCHMARKS "Build benchmarks" OFF)

acompiler_try_enable_llvm(ACOMPILER_HAS_LLVM)
if(ACOMPILER_ENABLE_LLVM AND NOT ACOMPILER_HAS_LLVM)
  message(WARNING "LLVM/MLIR not found. Building lightweight skeleton only.")
endif()

add_subdirectory(lib)

if(ACOMPILER_BUILD_RUNTIME)
  add_subdirectory(runtime)
endif()

if(ACOMPILER_BUILD_TOOLS)
  add_subdirectory(tools)
endif()

if(ACOMPILER_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()

if(ACOMPILER_BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif()
