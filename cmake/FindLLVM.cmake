# FindLLVM.cmake
# Thin wrapper - prefer LLVM's config mode.
# Usage: Set LLVM_DIR to <llvm_build>/lib/cmake/llvm
# Then: find_package(LLVM REQUIRED CONFIG)
#
# If LLVM_DIR is not set, this module attempts to find it.
# For standalone build: cmake -DLLVM_DIR=/path/to/llvm/build/lib/cmake/llvm ..
#
# LLVM and MLIR are typically built together in llvm-project.
# MLIR_DIR would be: <llvm_build>/lib/cmake/mlir

if(NOT LLVM_DIR)
  # Try common locations
  set(_llvm_paths
    "$ENV{LLVM_DIR}"
    "$ENV{LLVM_ROOT}/lib/cmake/llvm"
    "/usr/lib/cmake/llvm"
    "/usr/local/lib/cmake/llvm"
  )
  foreach(_p IN LISTS _llvm_paths)
    if(EXISTS "${_p}/LLVMConfig.cmake")
      set(LLVM_DIR "${_p}" CACHE PATH "LLVM cmake config directory")
      break()
    endif()
  endforeach()
endif()

# Actual find is done by LLVM's config; this file documents the convention.
# Root CMakeLists.txt uses: find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_DIR})
