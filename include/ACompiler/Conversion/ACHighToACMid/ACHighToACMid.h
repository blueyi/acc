//===- ACHighToACMid.h - ACHigh to ACMid Lowering ---------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_CONVERSION_ACHIGHTOACMID_H
#define ACOMPILER_CONVERSION_ACHIGHTOACMID_H

#include <memory>

namespace mlir {
class Pass;
} // namespace mlir

namespace acompiler {

/// Create a pass to lower ACHigh dialect to ACMid dialect.
/// This converts graph-level operations to tile/loop-level representations.
std::unique_ptr<mlir::Pass> createACHighToACMidPass();

} // namespace acompiler

#endif // ACOMPILER_CONVERSION_ACHIGHTOACMID_H
