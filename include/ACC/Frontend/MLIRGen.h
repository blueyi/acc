//===- MLIRGen.h - AST to MLIR Generation -----------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Converts the ACC AST to MLIR (ACHigh Dialect) representation.
//
//===----------------------------------------------------------------------===//

#ifndef ACC_FRONTEND_MLIRGEN_H
#define ACC_FRONTEND_MLIRGEN_H

#include "ACC/Frontend/AST.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"

namespace mlir {
class ModuleOp;
} // namespace mlir

namespace acc {

/// Generate MLIR (in ACHigh Dialect) from the AST.
mlir::OwningOpRef<mlir::ModuleOp> mlirGen(mlir::MLIRContext &context,
                                           ModuleAST &moduleAST);

} // namespace acc

#endif // ACC_FRONTEND_MLIRGEN_H
