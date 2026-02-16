#include "acompiler/InitACompiler.h"

#include "mlir/InitAllDialects.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Support/MlirOptMain.h"

int main(int argc, char **argv) {
  mlir::DialectRegistry registry;

  // Register upstream MLIR dialects/passes for convenience.
  mlir::registerAllDialects(registry);
  mlir::registerAllPasses();

  // Register ACompiler dialects/passes.
  acompiler::registerACompilerDialects(registry);
  acompiler::registerACompilerPasses();

  return mlir::asMainReturnCode(
      mlir::MlirOptMain(argc, argv, "ACompiler optimizer driver\n", registry));
}

