//===- ACMidOps.h - ACMid Dialect Operations --------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_DIALECT_ACMID_ACMIDOPS_H
#define ACOMPILER_DIALECT_ACMID_ACMIDOPS_H

#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#define GET_OP_CLASSES
#include "ACompiler/Dialect/ACMid/ACMidOps.h.inc"

#endif // ACOMPILER_DIALECT_ACMID_ACMIDOPS_H
