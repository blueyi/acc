//===- ACHighToACMid.h - ACHigh to ACMid Lowering ---------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief ACHigh to ACMid lowering pass. */

#ifndef ACC_CONVERSION_ACHIGHTOACMID_H
#define ACC_CONVERSION_ACHIGHTOACMID_H

#include <memory>

namespace mlir {
class Pass;
} // namespace mlir

namespace acc {

/// Create a pass to lower ACHigh dialect to ACMid dialect.
/// Converts graph-level operations to tile/loop-level representations.
/// \return A new ACHighToACMid pass.
std::unique_ptr<mlir::Pass> createACHighToACMidPass();

} // namespace acc

#endif // ACC_CONVERSION_ACHIGHTOACMID_H
