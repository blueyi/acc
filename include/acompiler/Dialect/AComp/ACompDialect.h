#pragma once

#include "mlir/IR/Dialect.h"

namespace acompiler {

/// Minimal starter dialect for ACompiler.
/// Namespace: "acomp"
class ACompDialect final : public mlir::Dialect {
public:
  explicit ACompDialect(mlir::MLIRContext *context);

  static mlir::StringRef getDialectNamespace() { return "acomp"; }
};

} // namespace acompiler

