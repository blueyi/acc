//===- Passes.h - ACompiler transform passes -------------------*- C++ -*-===//
//
// This file declares transform passes for ACompiler.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_TRANSFORMS_PASSES_H
#define ACOMPILER_TRANSFORMS_PASSES_H

#include "mlir/Pass/Pass.h"
#include <memory>

namespace mlir {

class Pass;

/// Create a pass to fuse Conv2D and ReLU operations.
std::unique_ptr<Pass> createConvReLUFusionPass();

/// Create a pass to perform constant folding for AI dialect.
std::unique_ptr<Pass> createAIConstantFoldingPass();

/// Create a pass to infer shapes for AI operations.
std::unique_ptr<Pass> createAIShapeInferencePass();

/// Generate the code for registering passes.
#define GEN_PASS_REGISTRATION
#include "acompiler/Transforms/Passes.h.inc"

} // namespace mlir

#endif // ACOMPILER_TRANSFORMS_PASSES_H
