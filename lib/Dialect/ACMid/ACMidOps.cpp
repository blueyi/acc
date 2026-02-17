//===- ACMidOps.cpp - ACMid Dialect Operations Implementation ---*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Dialect/ACMid/ACMidOps.h"
#include "ACC/Dialect/ACMid/ACMidDialect.h"
#include "mlir/IR/Builders.h"

using namespace mlir;
using namespace acc::acmid;

#define GET_OP_CLASSES
#include "ACC/Dialect/ACMid/ACMidOps.cpp.inc"
