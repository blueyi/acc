//===- ACHighDialect.cpp - ACHigh Dialect Registration -----------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Dialect/ACHigh/ACHighDialect.h"
#include "ACC/Dialect/ACHigh/ACHighOps.h"

using namespace mlir;
using namespace acc::achigh;

#include "ACC/Dialect/ACHigh/ACHighDialect.cpp.inc"

void ACHighDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "ACC/Dialect/ACHigh/ACHighOps.cpp.inc"
      >();
}
