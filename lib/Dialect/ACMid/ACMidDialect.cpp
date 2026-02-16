//===- ACMidDialect.cpp - ACMid Dialect Registration ------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Dialect/ACMid/ACMidDialect.h"
#include "ACompiler/Dialect/ACMid/ACMidOps.h"

using namespace mlir;
using namespace acompiler::acmid;

#include "ACompiler/Dialect/ACMid/ACMidDialect.cpp.inc"

void ACMidDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "ACompiler/Dialect/ACMid/ACMidOps.cpp.inc"
      >();
}
