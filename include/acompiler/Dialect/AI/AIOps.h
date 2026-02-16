//===- AIOps.h - AI dialect operations --------------------------*- C++ -*-===//
//
// This file defines operations in the AI dialect.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_DIALECT_AI_AIOPS_H
#define ACOMPILER_DIALECT_AI_AIOPS_H

#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#define GET_OP_CLASSES
#include "acompiler/Dialect/AI/AIOps.h.inc"

#endif // ACOMPILER_DIALECT_AI_AIOPS_H
