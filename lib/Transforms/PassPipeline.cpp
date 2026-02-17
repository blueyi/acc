//===- PassPipeline.cpp - MLIR Pass Pipeline Configuration ------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Defines the default MLIR optimization pass pipeline for ACC.
//
//===----------------------------------------------------------------------===//

#include "ACC/Transforms/Passes.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Transforms/Passes.h"

using namespace mlir;

namespace acc {

void registerTransformPasses() {
  // Register all ACC transform passes so they can be used via
  // command-line options in tools like ac-opt.
  PassRegistration<>();
  // TODO: Register individual passes using PassRegistration
}

/// Build the default MLIR optimization pipeline.
/// The ordering of passes matters for optimization effectiveness:
///   1. Shape inference (enable subsequent optimizations)
///   2. Constant folding (simplify the graph)
///   3. Dead code elimination (clean up after folding)
///   4. Operator fusion (reduce memory traffic)
///   5. Layout transform (prepare for target architecture)
void buildDefaultMLIRPipeline(OpPassManager &pm) {
  // TODO: Add passes to the pipeline
  // pm.addPass(createShapeInferencePass());
  // pm.addPass(createConstantFoldingPass());
  // pm.addPass(createCanonicalizerPass());
  // pm.addPass(createDeadCodeEliminationPass());
  // pm.addPass(createOpFusionPass());
  // pm.addPass(createLayoutTransformPass());
}

} // namespace acc
