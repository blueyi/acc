//===- ONNXImporter.h - ONNX Model Importer ---------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Imports ONNX model files and converts them to MLIR (ACHigh Dialect).
//
//===----------------------------------------------------------------------===//

#ifndef ACC_FRONTEND_ONNXIMPORTER_H
#define ACC_FRONTEND_ONNXIMPORTER_H

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"
#include <string>

namespace mlir {
class ModuleOp;
} // namespace mlir

namespace acc {

/// Import an ONNX model file and convert it to MLIR (ACHigh Dialect).
/// Returns the MLIR module or nullptr on failure.
mlir::OwningOpRef<mlir::ModuleOp> importONNXModel(mlir::MLIRContext &context,
                                                    const std::string &path);

} // namespace acc

#endif // ACC_FRONTEND_ONNXIMPORTER_H
