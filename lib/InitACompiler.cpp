#include "acompiler/InitACompiler.h"

#include "acompiler/Dialect/AComp/ACompDialect.h"

#include "mlir/IR/DialectRegistry.h"

namespace acompiler {

void registerACompilerDialects(mlir::DialectRegistry &registry) {
  registry.insert<ACompDialect>();
}

void registerACompilerPasses() {
  // Pass registrations will be added in later milestones.
}

} // namespace acompiler

