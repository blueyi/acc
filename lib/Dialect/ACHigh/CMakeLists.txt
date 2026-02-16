add_mlir_dialect_library(ACDialectACHigh
  ACHighDialect.cpp
  ACHighOps.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACompiler/Dialect/ACHigh

  DEPENDS
  ACHighOpsIncGen

  LINK_LIBS PUBLIC
  MLIRIR
  MLIRSupport
  MLIRInferTypeOpInterface
  MLIRSideEffectInterfaces
)
