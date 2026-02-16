add_mlir_dialect_library(ACConversionACMidToLinalg
  ACMidToLinalg.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACompiler/Conversion/ACMidToLinalg

  DEPENDS
  ACMidOpsIncGen

  LINK_LIBS PUBLIC
  ACDialectACMid
  MLIRIR
  MLIRPass
  MLIRLinalg
  MLIRTransformUtils
)
