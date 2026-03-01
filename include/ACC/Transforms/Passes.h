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

/** \file
 * \brief ACC MLIR transform pass constructors and registration. */

#ifndef ACC_TRANSFORMS_PASSES_H
#define ACC_TRANSFORMS_PASSES_H

#include <memory>

namespace mlir {
class Pass;
} // namespace mlir

namespace acc {

/// Creates a pass that performs constant folding on ACHigh operations.
/// \return A new constant folding pass.
std::unique_ptr<mlir::Pass> createConstantFoldingPass();

/// Creates a pass that fuses compatible operations (e.g., Conv+BN+ReLU).
/// \return A new op fusion pass.
std::unique_ptr<mlir::Pass> createOpFusionPass();

/// Creates a pass that infers tensor shapes through the computation graph.
/// \return A new shape inference pass.
std::unique_ptr<mlir::Pass> createShapeInferencePass();

/// Creates a pass that optimizes data layout transformations (NCHW/NHWC).
/// \return A new layout transform pass.
std::unique_ptr<mlir::Pass> createLayoutTransformPass();

/// Creates a pass that eliminates dead/unreachable code.
/// \return A new dead code elimination pass.
std::unique_ptr<mlir::Pass> createDeadCodeEliminationPass();

/// Register all ACC MLIR transform passes with the global registry.
void registerTransformPasses();

} // namespace acc

#endif // ACC_TRANSFORMS_PASSES_H
