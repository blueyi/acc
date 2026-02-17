add_llvm_library(ACCodeGen
  CodeGenDriver.cpp
  CPUCodeGen.cpp
  JITEngine.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACC/CodeGen

  LINK_LIBS
  LLVMCore
  LLVMSupport
  LLVMTarget
  LLVMMC
  LLVMOrcJIT
  LLVMCodeGen
  LLVMAnalysis
  LLVMX86CodeGen
  LLVMX86AsmParser
  LLVMAArch64CodeGen
  LLVMAArch64AsmParser
)
