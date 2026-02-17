//===- ACMidToLLVM.cpp - ACMid to LLVM Dialect Lowering ----------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Conversion/ACMidToLLVM/ACMidToLLVM.h"
#include "mlir/Conversion/LLVMCommon/ConversionTarget.h"
#include "mlir/Conversion/LLVMCommon/TypeConverter.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"

using namespace mlir;

namespace {

struct ACMidToLLVMPass
    : public PassWrapper<ACMidToLLVMPass, OperationPass<ModuleOp>> {
  StringRef getArgument() const final { return "ac-mid-to-llvm"; }
  StringRef getDescription() const final {
    return "Lower ACMid dialect (via Linalg/Affine/SCF) to LLVM dialect";
  }

  void runOnOperation() override {
    // TODO: Implement the full lowering pipeline:
    // 1. Bufferize tensors to memrefs
    // 2. Lower Linalg to Affine/SCF loops
    // 3. Lower Affine to SCF
    // 4. Lower SCF to CF (Control Flow)
    // 5. Lower CF + Arith + MemRef to LLVM dialect
  }
};

} // namespace

std::unique_ptr<mlir::Pass> acc::createACMidToLLVMPass() {
  return std::make_unique<ACMidToLLVMPass>();
}
