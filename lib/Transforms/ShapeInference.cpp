//===- ShapeInference.cpp - Shape Inference Pass ----------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Transforms/Passes.h"
#include "ACC/Dialect/ACHigh/ACHighOps.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Support/TypeID.h"

using namespace mlir;

namespace {

struct ShapeInferencePass
    : public PassWrapper<ShapeInferencePass, OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(ShapeInferencePass)

  StringRef getArgument() const final { return "ac-shape-inference"; }
  StringRef getDescription() const final {
    return "Infer tensor shapes through the computation graph";
  }

  void runOnOperation() override {
    // TODO: Implement shape inference
    // 1. Propagate input shapes forward through each operation
    // 2. Handle Conv2D output shape calculation based on strides/padding
    // 3. Handle MatMul shape propagation
    // 4. Handle reshape/broadcast shape inference
  }
};

} // namespace

std::unique_ptr<mlir::Pass> acc::createShapeInferencePass() {
  return std::make_unique<ShapeInferencePass>();
}
