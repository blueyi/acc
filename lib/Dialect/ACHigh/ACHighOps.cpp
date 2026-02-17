//===- ACHighOps.cpp - ACHigh Dialect Operations Implementation --*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Dialect/ACHigh/ACHighOps.h"
#include "ACC/Dialect/ACHigh/ACHighDialect.h"
#include "mlir/IR/Builders.h"

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

mlir::LogicalResult Conv2DOp::verify() { return mlir::success(); }

mlir::OpFoldResult Conv2DOp::fold(FoldAdaptor adaptor) { return {}; }

mlir::LogicalResult Conv2DOp::inferReturnTypes(
    mlir::MLIRContext *context, std::optional<mlir::Location> location,
    mlir::ValueRange operands, mlir::DictionaryAttr attributes,
    mlir::OpaqueProperties properties, mlir::RegionRange regions,
    llvm::SmallVectorImpl<mlir::Type> &inferredReturnTypes) {
  if (operands.empty())
    return mlir::failure();

  auto inputType = mlir::dyn_cast<mlir::RankedTensorType>(operands.front().getType());
  if (!inputType)
    return mlir::failure();

  inferredReturnTypes.push_back(inputType);
  return mlir::success();
}
