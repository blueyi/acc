add_llvm_library(ACIROptimizer
  CustomLoopOpt.cpp
  CustomVectorize.cpp
  PassPipeline.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACompiler/IROptimizer

  LINK_LIBS
  LLVMCore
  LLVMSupport
  LLVMPasses
  LLVMAnalysis
  LLVMTransformUtils
)
