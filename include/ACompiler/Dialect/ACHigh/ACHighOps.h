//===- ACHighOps.h - ACHigh Dialect Operations ------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_DIALECT_ACHIGH_ACHIGHOPS_H
#define ACOMPILER_DIALECT_ACHIGH_ACHIGHOPS_H

#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#define GET_OP_CLASSES
#include "ACompiler/Dialect/ACHigh/ACHighOps.h.inc"

#endif // ACOMPILER_DIALECT_ACHIGH_ACHIGHOPS_H
