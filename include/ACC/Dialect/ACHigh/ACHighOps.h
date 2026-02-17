//===- ACHighOps.h - ACHigh Dialect Operations ------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef ACC_DIALECT_ACHIGH_ACHIGHOPS_H
#define ACC_DIALECT_ACHIGH_ACHIGHOPS_H

#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#define GET_OP_CLASSES
#include "ACC/Dialect/ACHigh/ACHighOps.h.inc"

#endif // ACC_DIALECT_ACHIGH_ACHIGHOPS_H
