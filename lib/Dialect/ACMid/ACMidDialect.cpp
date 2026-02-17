//===- ACMidDialect.cpp - ACMid Dialect Registration ------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Dialect/ACMid/ACMidDialect.h"
#include "ACC/Dialect/ACMid/ACMidOps.h"

using namespace mlir;
using namespace acc::acmid;

#include "ACC/Dialect/ACMid/ACMidDialect.cpp.inc"

void ACMidDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "ACC/Dialect/ACMid/ACMidOps.cpp.inc"
      >();
}
