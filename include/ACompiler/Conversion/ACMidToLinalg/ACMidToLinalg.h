//===- ACMidToLinalg.h - ACMid to Linalg Lowering ---------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_CONVERSION_ACMIDTOLINALG_H
#define ACOMPILER_CONVERSION_ACMIDTOLINALG_H

#include <memory>

namespace mlir {
class Pass;
} // namespace mlir

namespace acompiler {

/// Create a pass to lower ACMid dialect to MLIR Linalg dialect.
/// This converts tile-level operations to Linalg generic operations.
std::unique_ptr<mlir::Pass> createACMidToLinalgPass();

} // namespace acompiler

#endif // ACOMPILER_CONVERSION_ACMIDTOLINALG_H
