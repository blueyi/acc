//===- ACHighOps.cpp - ACHigh Dialect Operations Implementation --*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Dialect/ACHigh/ACHighOps.h"
#include "ACC/Dialect/ACHigh/ACHighDialect.h"

using namespace mlir;
using namespace acc::achigh;

#define GET_OP_CLASSES
#include "ACC/Dialect/ACHigh/ACHighOps.cpp.inc"

//===----------------------------------------------------------------------===//
// ConstantOp - Fold
//===----------------------------------------------------------------------===//

OpFoldResult ConstantOp::fold(FoldAdaptor adaptor) {
  return getValueAttr();
}
