//===- main.cpp - ACC MLIR Optimizer Tool -----------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// ac-opt is analogous to mlir-opt. It provides a command-line interface for
// running MLIR passes on .mlir files. This is the primary tool for developing
// and testing MLIR transformations.
//
// Usage:
//   ac-opt input.mlir --ac-op-fusion --ac-constant-folding
//   ac-opt input.mlir --ac-shape-inference -o output.mlir
//   ac-opt input.mlir --pass-pipeline="func.func(ac-op-fusion,ac-constant-folding)"
//
//===----------------------------------------------------------------------===//

#include "ACC/Conversion/ACHighToACMid/ACHighToACMid.h"
#include "ACC/InitAllDialects.h"
#include "ACC/Transforms/Passes.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/DialectRegistry.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassRegistry.h"
#include "mlir/Tools/mlir-opt/MlirOptMain.h"

int main(int argc, char **argv) {
  mlir::DialectRegistry registry;

  // Register all ACC dialects
  acc::registerAllDialects(registry);

  // Register standard MLIR dialects used by test/input MLIR (e.g. func.func)
  registry.insert<mlir::func::FuncDialect>();

  // Register ACC transform passes (e.g. --ac-op-fusion, --ac-constant-folding)
  acc::registerTransformPasses();

  // Register ACC conversion passes (e.g. --ac-high-to-mid)
  mlir::registerPass(acc::createACHighToACMidPass);

  return mlir::asMainReturnCode(
      mlir::MlirOptMain(argc, argv, "ACC MLIR optimizer\n", registry));
}
