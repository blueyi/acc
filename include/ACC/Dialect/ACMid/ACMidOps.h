//===- ACMidOps.h - ACMid Dialect Operations --------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief ACMid dialect operations (TableGen). */

#ifndef ACC_DIALECT_ACMID_ACMIDOPS_H
#define ACC_DIALECT_ACMID_ACMIDOPS_H

#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#define GET_OP_CLASSES
#include "ACC/Dialect/ACMid/ACMidOps.h.inc"

#endif // ACC_DIALECT_ACMID_ACMIDOPS_H
