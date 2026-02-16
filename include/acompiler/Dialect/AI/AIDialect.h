//===- AIDialect.h - AI dialect definition ----------------------*- C++ -*-===//
//
// This file defines the AI dialect.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_DIALECT_AI_AIDIALECT_H
#define ACOMPILER_DIALECT_AI_AIDIALECT_H

#include "mlir/IR/Dialect.h"

namespace mlir {
namespace ai {

/// Include the auto-generated header file containing the declaration of the AI
/// dialect.
#include "acompiler/Dialect/AI/AIDialect.h.inc"

} // namespace ai
} // namespace mlir

#endif // ACOMPILER_DIALECT_AI_AIDIALECT_H
