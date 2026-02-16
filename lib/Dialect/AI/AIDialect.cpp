//===- AIDialect.cpp - AI dialect -------------------------------*- C++ -*-===//
//
// This file implements the AI dialect.
//
//===----------------------------------------------------------------------===//

#include "acompiler/Dialect/AI/AIDialect.h"
#include "acompiler/Dialect/AI/AIOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/DialectImplementation.h"

using namespace mlir;
using namespace mlir::ai;

//===----------------------------------------------------------------------===//
// AI Dialect
//===----------------------------------------------------------------------===//

#include "acompiler/Dialect/AI/AIDialect.cpp.inc"

void AIDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "acompiler/Dialect/AI/AIOps.cpp.inc"
      >();
}
