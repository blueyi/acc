//===- ACMidToLLVM.h - ACMid to LLVM Lowering -------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef ACC_CONVERSION_ACMIDTOLLVM_H
#define ACC_CONVERSION_ACMIDTOLLVM_H

#include <memory>

namespace mlir {
class Pass;
} // namespace mlir

namespace acc {

/// Create a pass to lower from ACMid (via Linalg/Affine/SCF) to LLVM dialect.
/// This represents the final MLIR lowering step before LLVM IR emission.
std::unique_ptr<mlir::Pass> createACMidToLLVMPass();

} // namespace acc

#endif // ACC_CONVERSION_ACMIDTOLLVM_H
