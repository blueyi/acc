//===- ONNXImporter.h - ONNX Model Importer ---------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Imports ONNX model files and converts them to MLIR (ACHigh Dialect).
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_FRONTEND_ONNXIMPORTER_H
#define ACOMPILER_FRONTEND_ONNXIMPORTER_H

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"
#include <string>

namespace mlir {
class ModuleOp;
} // namespace mlir

namespace acompiler {

/// Import an ONNX model file and convert it to MLIR (ACHigh Dialect).
/// Returns the MLIR module or nullptr on failure.
mlir::OwningOpRef<mlir::ModuleOp> importONNXModel(mlir::MLIRContext &context,
                                                    const std::string &path);

} // namespace acompiler

#endif // ACOMPILER_FRONTEND_ONNXIMPORTER_H
