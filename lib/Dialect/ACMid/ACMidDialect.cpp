//===- ACMidDialect.cpp - ACMid Dialect Registration ------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Dialect/ACMid/ACMidDialect.h"
#include "ACC/Dialect/ACMid/ACMidOps.h"
#include "mlir/IR/DialectImplementation.h"

using namespace mlir;
using namespace acc::acmid;

#include "ACC/Dialect/ACMid/ACMidDialect.cpp.inc"

void ACMidDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "ACC/Dialect/ACMid/ACMidOps.cpp.inc"
      >();
}

mlir::Attribute ACMidDialect::parseAttribute(mlir::DialectAsmParser &parser,
                                             mlir::Type type) const {
  parser.emitError(parser.getCurrentLocation(),
                   "ACMid attributes are not implemented yet");
  return {};
}

void ACMidDialect::printAttribute(mlir::Attribute attr,
                                  mlir::DialectAsmPrinter &printer) const {
  (void)attr;
  // Keep printer non-fatal until custom attrs are introduced.
  printer << "<acmid.attr.unimplemented>";
}

mlir::Type ACMidDialect::parseType(mlir::DialectAsmParser &parser) const {
  parser.emitError(parser.getCurrentLocation(),
                   "ACMid types are not implemented yet");
  return {};
}

void ACMidDialect::printType(mlir::Type type,
                             mlir::DialectAsmPrinter &printer) const {
  (void)type;
  // Keep printer non-fatal until custom types are introduced.
  printer << "<acmid.type.unimplemented>";
}
