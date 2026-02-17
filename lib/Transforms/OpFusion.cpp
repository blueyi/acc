//===- OpFusion.cpp - Operator Fusion Pass ----------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Fuses compatible adjacent operations to reduce memory traffic.
// Common fusion patterns: Conv+BN+ReLU, MatMul+Add+ReLU, etc.
//
//===----------------------------------------------------------------------===//

#include "ACC/Transforms/Passes.h"
#include "ACC/Dialect/ACHigh/ACHighOps.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Support/TypeID.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

using namespace mlir;

namespace {

struct OpFusionPass
    : public PassWrapper<OpFusionPass, OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(OpFusionPass)

  StringRef getArgument() const final { return "ac-op-fusion"; }
  StringRef getDescription() const final {
    return "Fuse compatible adjacent operations";
  }

  void runOnOperation() override {
    // TODO: Implement fusion patterns
    // 1. Conv2D + ReLU → FusedConv2DReLU
    // 2. MatMul + Add (bias) → FusedMatMulBias
    // 3. Conv2D + BatchNorm + ReLU → FusedConvBNReLU
  }
};

} // namespace

std::unique_ptr<mlir::Pass> acc::createOpFusionPass() {
  return std::make_unique<OpFusionPass>();
}
