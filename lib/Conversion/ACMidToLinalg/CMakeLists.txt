add_mlir_dialect_library(ACConversionACMidToLinalg
  ACMidToLinalg.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACC/Conversion/ACMidToLinalg

  DEPENDS
  ACMidOpsIncGen

  LINK_LIBS PUBLIC
  ACDialectACMid
  MLIRIR
  MLIRPass
  MLIRLinalgDialect
  MLIRTransformUtils
)
