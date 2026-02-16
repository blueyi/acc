//===- ACHighDialect.cpp - ACHigh Dialect Registration -----------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Dialect/ACHigh/ACHighDialect.h"
#include "ACompiler/Dialect/ACHigh/ACHighOps.h"

using namespace mlir;
using namespace acompiler::achigh;

#include "ACompiler/Dialect/ACHigh/ACHighDialect.cpp.inc"

void ACHighDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "ACompiler/Dialect/ACHigh/ACHighOps.cpp.inc"
      >();
}
