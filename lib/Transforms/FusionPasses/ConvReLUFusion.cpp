//===- ConvReLUFusion.cpp - Conv2D + ReLU fusion pass ----------*- C++ -*-===//
//
// This file implements the Conv2D + ReLU fusion pass.
//
//===----------------------------------------------------------------------===//

#include "acompiler/Dialect/AI/AIDialect.h"
#include "acompiler/Dialect/AI/AIOps.h"
#include "acompiler/Transforms/Passes.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

namespace mlir {

#define GEN_PASS_DEF_CONVRELUFUSION
#include "acompiler/Transforms/Passes.h.inc"

namespace {

/// Pattern to fuse Conv2D + ReLU into ConvReLU
struct ConvReLUFusionPattern : public OpRewritePattern<ai::ReLUOp> {
  using OpRewritePattern<ai::ReLUOp>::OpRewritePattern;

  LogicalResult matchAndRewrite(ai::ReLUOp reluOp,
                                PatternRewriter &rewriter) const override {
    // Check if the input to ReLU is from Conv2D
    auto convOp = reluOp.getInput().getDefiningOp<ai::Conv2DOp>();
    if (!convOp)
      return failure();

    // Check if Conv2D has only one use (the ReLU)
    if (!convOp->hasOneUse())
      return failure();

    // Create the fused ConvReLU operation
    rewriter.replaceOpWithNewOp<ai::ConvReLUOp>(
        reluOp, reluOp.getType(), convOp.getInput(), convOp.getWeight(),
        convOp.getStrideAttr(), convOp.getPaddingAttr());

    return success();
  }
};

/// Pass implementation
struct ConvReLUFusionPass
    : public impl::ConvReLUFusionBase<ConvReLUFusionPass> {
  void runOnOperation() override {
    RewritePatternSet patterns(&getContext());
    patterns.add<ConvReLUFusionPattern>(&getContext());

    if (failed(applyPatternsAndFoldGreedily(getOperation(),
                                            std::move(patterns)))) {
      signalPassFailure();
    }
  }
};

} // namespace

std::unique_ptr<Pass> createConvReLUFusionPass() {
  return std::make_unique<ConvReLUFusionPass>();
}

} // namespace mlir
