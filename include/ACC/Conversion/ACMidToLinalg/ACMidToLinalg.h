//===- ACMidToLinalg.h - ACMid to Linalg Lowering ---------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief ACMid to Linalg lowering pass. */

#ifndef ACC_CONVERSION_ACMIDTOLINALG_H
#define ACC_CONVERSION_ACMIDTOLINALG_H

#include <memory>

namespace mlir {
class Pass;
} // namespace mlir

namespace acc {

/// Create a pass to lower ACMid dialect to MLIR Linalg dialect.
/// Converts tile-level operations to Linalg generic operations.
/// \return A new ACMidToLinalg pass.
std::unique_ptr<mlir::Pass> createACMidToLinalgPass();

} // namespace acc

#endif // ACC_CONVERSION_ACMIDTOLINALG_H
