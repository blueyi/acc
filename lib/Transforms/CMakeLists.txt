add_mlir_dialect_library(ACTransforms
  ConstantFolding.cpp
  OpFusion.cpp
  ShapeInference.cpp
  LayoutTransform.cpp
  DeadCodeElimination.cpp
  PassPipeline.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACompiler/Transforms

  DEPENDS
  ACHighOpsIncGen

  LINK_LIBS PUBLIC
  ACDialectACHigh
  MLIRIR
  MLIRPass
  MLIRTransformUtils
)
