//===- AIOps.cpp - AI dialect operations ------------------------*- C++ -*-===//
//
// This file implements operations in the AI dialect.
//
//===----------------------------------------------------------------------===//

#include "acompiler/Dialect/AI/AIOps.h"
#include "acompiler/Dialect/AI/AIDialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/OpImplementation.h"

using namespace mlir;
using namespace mlir::ai;

#define GET_OP_CLASSES
#include "acompiler/Dialect/AI/AIOps.cpp.inc"

//===----------------------------------------------------------------------===//
// Conv2DOp
//===----------------------------------------------------------------------===//

// Additional verification or methods can be added here

//===----------------------------------------------------------------------===//
// MatMulOp
//===----------------------------------------------------------------------===//

// Additional verification or methods can be added here

//===----------------------------------------------------------------------===//
// ReLUOp
//===----------------------------------------------------------------------===//

// Additional verification or methods can be added here
