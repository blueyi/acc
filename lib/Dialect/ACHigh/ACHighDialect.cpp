//===- ACHighDialect.cpp - ACHigh Dialect Registration -----------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Dialect/ACHigh/ACHighDialect.h"
#include "ACC/Dialect/ACHigh/ACHighOps.h"
#include "mlir/IR/DialectImplementation.h"

using namespace mlir;
using namespace acc::achigh;

#include "ACC/Dialect/ACHigh/ACHighDialect.cpp.inc"

void ACHighDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "ACC/Dialect/ACHigh/ACHighOps.cpp.inc"
      >();
}

mlir::Attribute ACHighDialect::parseAttribute(mlir::DialectAsmParser &parser,
                                              mlir::Type type) const {
  parser.emitError(parser.getCurrentLocation(),
                   "ACHigh attributes are not implemented yet");
  return {};
}

void ACHighDialect::printAttribute(mlir::Attribute attr,
                                   mlir::DialectAsmPrinter &printer) const {
  (void)attr;
  // Keep printer non-fatal until custom attrs are introduced.
  printer << "<achigh.attr.unimplemented>";
}

mlir::Type ACHighDialect::parseType(mlir::DialectAsmParser &parser) const {
  parser.emitError(parser.getCurrentLocation(),
                   "ACHigh types are not implemented yet");
  return {};
}

void ACHighDialect::printType(mlir::Type type,
                              mlir::DialectAsmPrinter &printer) const {
  (void)type;
  // Keep printer non-fatal until custom types are introduced.
  printer << "<achigh.type.unimplemented>";
}
