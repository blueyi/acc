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
//   ac-opt input.mlir --ac-high-to-mid
//   ac-opt input.mlir --ac-shape-inference -o output.mlir
//
//===----------------------------------------------------------------------===//

#include "ACC/InitAllDialects.h"
#include "mlir/IR/DialectRegistry.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Tools/mlir-opt/MlirOptMain.h"

int main(int argc, char **argv) {
  mlir::DialectRegistry registry;

  // Register all ACC dialects
  acc::registerAllDialects(registry);

  // Register standard MLIR dialects needed for lowering
  // TODO: Register func, linalg, affine, scf, arith, memref, llvm dialects

  return mlir::asMainReturnCode(
      mlir::MlirOptMain(argc, argv, "ACC MLIR optimizer\n", registry));
}
