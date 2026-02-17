add_mlir_dialect_library(ACConversionACMidToLLVM
  ACMidToLLVM.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACC/Conversion/ACMidToLLVM

  DEPENDS
  ACMidOpsIncGen

  LINK_LIBS PUBLIC
  ACDialectACMid
  MLIRIR
  MLIRPass
  MLIRLLVMDialect
  MLIRLLVMCommonConversion
  MLIRTransformUtils
)
