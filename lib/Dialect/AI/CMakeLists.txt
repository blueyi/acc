add_mlir_dialect_library(ACompilerAIDialect
  AIDialect.cpp
  AIOps.cpp
  
  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/acompiler/Dialect/AI
  
  DEPENDS
  MLIRAIIncGen
  
  LINK_LIBS PUBLIC
  MLIRIR
  MLIRSupport
  )
