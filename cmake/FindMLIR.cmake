# FindMLIR.cmake
# MLIR is built as part of LLVM in llvm-project.
# Usage: Set MLIR_DIR to <llvm_build>/lib/cmake/mlir
# Then: find_package(MLIR REQUIRED CONFIG)
#
# Typically when building llvm-project with MLIR enabled:
#   -DLLVM_ENABLE_PROJECTS="mlir"
# The build produces:
#   <build>/lib/cmake/llvm/LLVMConfig.cmake
#   <build>/lib/cmake/mlir/MLIRConfig.cmake
#
# For ACompiler: pass both
#   -DLLVM_DIR=<build>/lib/cmake/llvm
#   -DMLIR_DIR=<build>/lib/cmake/mlir

if(NOT MLIR_DIR AND LLVM_DIR)
  get_filename_component(_llvm_parent ${LLVM_DIR} DIRECTORY)
  if(EXISTS "${_llvm_parent}/mlir/MLIRConfig.cmake")
    set(MLIR_DIR "${_llvm_parent}/mlir" CACHE PATH "MLIR cmake config directory")
  endif()
  unset(_llvm_parent)
endif()

# Actual find is done by MLIR's config.
# Root CMakeLists.txt uses: find_package(MLIR REQUIRED CONFIG PATHS ${MLIR_DIR})
