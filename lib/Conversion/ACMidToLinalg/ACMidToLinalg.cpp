//===- ACMidToLinalg.cpp - ACMid to Linalg Lowering -------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Conversion/ACMidToLinalg/ACMidToLinalg.h"
#include "ACC/Dialect/ACMid/ACMidDialect.h"
#include "ACC/Dialect/ACMid/ACMidOps.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Support/TypeID.h"
#include "mlir/Transforms/DialectConversion.h"

using namespace mlir;

namespace {

struct ACMidToLinalgPass
    : public PassWrapper<ACMidToLinalgPass, OperationPass<ModuleOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(ACMidToLinalgPass)

  StringRef getArgument() const final { return "ac-mid-to-linalg"; }
  StringRef getDescription() const final {
    return "Lower ACMid dialect to MLIR Linalg dialect";
  }

  void runOnOperation() override {
    // TODO: Implement lowering patterns
    // 1. Convert acmid.tiled_matmul → linalg.matmul
    // 2. Convert acmid.tiled_conv2d → linalg.conv_2d_nhwc_hwcf
    // 3. Convert acmid.elem_unary/elem_binary → linalg.generic
  }
};

} // namespace

std::unique_ptr<mlir::Pass> acc::createACMidToLinalgPass() {
  return std::make_unique<ACMidToLinalgPass>();
}
