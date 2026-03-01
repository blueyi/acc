//===- ACHighToACMid.cpp - ACHigh to ACMid Lowering --------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// This file implements the lowering from ACHigh dialect to ACMid dialect.
// Graph-level operations are decomposed into tile/loop-level representations.
//
//===----------------------------------------------------------------------===//

#include "ACC/Conversion/ACHighToACMid/ACHighToACMid.h"
#include "ACC/Dialect/ACHigh/ACHighDialect.h"
#include "ACC/Dialect/ACHigh/ACHighOps.h"
#include "ACC/Dialect/ACMid/ACMidDialect.h"
#include "ACC/Dialect/ACMid/ACMidOps.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Support/TypeID.h"
#include "mlir/Transforms/DialectConversion.h"

using namespace mlir;

namespace {

struct ACHighToACMidPass
    : public PassWrapper<ACHighToACMidPass, OperationPass<ModuleOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(ACHighToACMidPass)

  StringRef getArgument() const final { return "ac-high-to-mid"; }
  StringRef getDescription() const final {
    return "Lower ACHigh dialect to ACMid dialect";
  }

  void runOnOperation() override {
    // TODO: Implement lowering patterns
    // 1. Convert achigh.matmul → acmid.tiled_matmul
    // 2. Convert achigh.conv2d → acmid.tiled_conv2d
    // 3. Convert achigh.relu/add → acmid.elem_unary/elem_binary
  }
};

} // namespace

std::unique_ptr<mlir::Pass> acc::createACHighToACMidPass() {
  return std::make_unique<ACHighToACMidPass>();
}
