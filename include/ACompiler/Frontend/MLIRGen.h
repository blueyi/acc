//===- MLIRGen.h - AST to MLIR Generation -----------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Converts the ACompiler AST to MLIR (ACHigh Dialect) representation.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_FRONTEND_MLIRGEN_H
#define ACOMPILER_FRONTEND_MLIRGEN_H

#include "ACompiler/Frontend/AST.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"

namespace mlir {
class ModuleOp;
} // namespace mlir

namespace acompiler {

/// Generate MLIR (in ACHigh Dialect) from the AST.
mlir::OwningOpRef<mlir::ModuleOp> mlirGen(mlir::MLIRContext &context,
                                           ModuleAST &moduleAST);

} // namespace acompiler

#endif // ACOMPILER_FRONTEND_MLIRGEN_H
