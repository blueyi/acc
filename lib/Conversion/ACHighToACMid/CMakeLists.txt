add_mlir_dialect_library(ACConversionACHighToACMid
  ACHighToACMid.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACC/Conversion/ACHighToACMid

  DEPENDS
  ACHighOpsIncGen
  ACMidOpsIncGen

  LINK_LIBS PUBLIC
  ACDialectACHigh
  ACDialectACMid
  MLIRIR
  MLIRPass
  MLIRTransformUtils
)
