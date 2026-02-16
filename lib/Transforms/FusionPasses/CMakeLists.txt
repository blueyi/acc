add_mlir_library(ACompilerFusionPasses
  ConvReLUFusion.cpp
  
  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/acompiler/Transforms
  
  DEPENDS
  ACompilerTransformsPassIncGen
  
  LINK_LIBS PUBLIC
  ACompilerAIDialect
  MLIRIR
  MLIRPass
  MLIRTransformUtils
  )
