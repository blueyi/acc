//===- main.cpp - ACC IR Translation Tool -----------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// ac-translate provides translation between different IR representations:
//   - MLIR (LLVM Dialect) → LLVM IR
//   - LLVM IR → MLIR (LLVM Dialect)
//
// Usage:
//   ac-translate --mlir-to-llvmir input.mlir -o output.ll
//   ac-translate --llvmir-to-mlir input.ll -o output.mlir
//
//===----------------------------------------------------------------------===//

#include "ACC/InitAllDialects.h"
#include "mlir/IR/DialectRegistry.h"
#include "mlir/Tools/mlir-translate/MlirTranslateMain.h"

int main(int argc, char **argv) {
  mlir::DialectRegistry registry;
  acc::registerAllDialects(registry);

  return mlir::asMainReturnCode(
      mlir::mlirTranslateMain(argc, argv, "ACC IR translator\n"));
}
