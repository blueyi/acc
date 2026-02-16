//===- ACHighOps.cpp - ACHigh Dialect Operations Implementation --*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Dialect/ACHigh/ACHighOps.h"
#include "ACompiler/Dialect/ACHigh/ACHighDialect.h"

using namespace mlir;
using namespace acompiler::achigh;

#define GET_OP_CLASSES
#include "ACompiler/Dialect/ACHigh/ACHighOps.cpp.inc"

//===----------------------------------------------------------------------===//
// ConstantOp - Fold
//===----------------------------------------------------------------------===//

OpFoldResult ConstantOp::fold(FoldAdaptor adaptor) {
  return getValueAttr();
}
