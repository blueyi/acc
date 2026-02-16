#pragma once

#include "mlir/IR/DialectRegistry.h"

namespace acompiler {

/// Register all ACompiler dialects into the given registry.
void registerACompilerDialects(mlir::DialectRegistry &registry);

/// Register all ACompiler passes (if any).
void registerACompilerPasses();

} // namespace acompiler

