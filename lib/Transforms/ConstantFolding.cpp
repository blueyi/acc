//===- ConstantFolding.cpp - Constant Folding Pass --------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Evaluates operations with constant operands at compile time.
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

struct ConstantFoldingPass
    : public PassWrapper<ConstantFoldingPass, OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(ConstantFoldingPass)

  StringRef getArgument() const final { return "ac-constant-folding"; }
  StringRef getDescription() const final {
    return "Fold constant expressions at compile time";
  }

  void runOnOperation() override {
    // TODO: Implement constant folding patterns
    // 1. Fold achigh.add with two constant operands
    // 2. Fold achigh.matmul with constant operands (small tensors)
    // 3. Propagate constants through reshape operations
  }
};

} // namespace

std::unique_ptr<mlir::Pass> acc::createConstantFoldingPass() {
  return std::make_unique<ConstantFoldingPass>();
}
