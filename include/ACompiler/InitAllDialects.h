//===- InitAllDialects.h - ACompiler Dialect Registration --------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// This file defines a helper to register all ACompiler dialects with an
// MLIR context. This is useful for tools like ac-opt that need all dialects.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_INITALLDIALECTS_H
#define ACOMPILER_INITALLDIALECTS_H

#include "ACompiler/Dialect/ACHigh/ACHighDialect.h"
#include "ACompiler/Dialect/ACMid/ACMidDialect.h"
#include "mlir/IR/DialectRegistry.h"

namespace acompiler {

/// Register all ACompiler dialects with the given registry.
inline void registerAllDialects(mlir::DialectRegistry &registry) {
  registry.insert<achigh::ACHighDialect>();
  registry.insert<acmid::ACMidDialect>();
}

} // namespace acompiler

#endif // ACOMPILER_INITALLDIALECTS_H
