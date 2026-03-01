//===- InitAllDialects.h - ACC Dialect Registration --------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// This file defines a helper to register all ACC dialects with an
// MLIR context. This is useful for tools like ac-opt that need all dialects.
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief Registration of all ACC MLIR dialects. */

#ifndef ACC_INITALLDIALECTS_H
#define ACC_INITALLDIALECTS_H

#include "ACC/Dialect/ACHigh/ACHighDialect.h"
#include "ACC/Dialect/ACMid/ACMidDialect.h"
#include "mlir/IR/DialectRegistry.h"

namespace acc {

/// Register all ACC dialects (ACHigh, ACMid) with the given registry.
/// \param registry Dialect registry to extend.
inline void registerAllDialects(mlir::DialectRegistry &registry) {
  registry.insert<achigh::ACHighDialect>();
  registry.insert<acmid::ACMidDialect>();
}

} // namespace acc

#endif // ACC_INITALLDIALECTS_H
