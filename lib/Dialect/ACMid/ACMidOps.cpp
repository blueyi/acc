//===- ACMidOps.cpp - ACMid Dialect Operations Implementation ---*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Dialect/ACMid/ACMidOps.h"
#include "ACompiler/Dialect/ACMid/ACMidDialect.h"

using namespace mlir;
using namespace acompiler::acmid;

#define GET_OP_CLASSES
#include "ACompiler/Dialect/ACMid/ACMidOps.cpp.inc"
