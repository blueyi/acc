//===- Passes.h - ACC MLIR Transform Passes ---------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// This header file defines prototypes that expose pass constructors for
// all ACC MLIR transformation passes.
//
//===----------------------------------------------------------------------===//

#ifndef ACC_TRANSFORMS_PASSES_H
#define ACC_TRANSFORMS_PASSES_H

#include <memory>

namespace mlir {
class Pass;
} // namespace mlir

namespace acc {

/// Creates a pass that performs constant folding on ACHigh operations.
std::unique_ptr<mlir::Pass> createConstantFoldingPass();

/// Creates a pass that fuses compatible operations (e.g., Conv+BN+ReLU).
std::unique_ptr<mlir::Pass> createOpFusionPass();

/// Creates a pass that infers tensor shapes through the computation graph.
std::unique_ptr<mlir::Pass> createShapeInferencePass();

/// Creates a pass that optimizes data layout transformations (NCHW/NHWC).
std::unique_ptr<mlir::Pass> createLayoutTransformPass();

/// Creates a pass that eliminates dead/unreachable code.
std::unique_ptr<mlir::Pass> createDeadCodeEliminationPass();

/// Register all ACC transform passes.
void registerTransformPasses();

} // namespace acc

#endif // ACC_TRANSFORMS_PASSES_H
