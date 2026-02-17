add_mlir_dialect_library(ACDialectACMid
  ACMidDialect.cpp
  ACMidOps.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACC/Dialect/ACMid

  DEPENDS
  ACMidOpsIncGen

  LINK_LIBS PUBLIC
  MLIRIR
  MLIRSupport
  MLIRSideEffectInterfaces
)
