//===- TorchImporter.h - PyTorch Model to ACC MLIR Importer -----*- C++ -*-===//
//
// Part of the ACC Project (AI Compiler Core)
//
//===----------------------------------------------------------------------===//
//
// Imports PyTorch models into ACC MLIR representation. Supports two pathways:
//
// Path 1: PyTorch → torch.export → Torch-MLIR → ACC MLIR (whole-graph)
// Path 2: PyTorch → torch.onnx.export → ONNX → ACC MLIR (via ONNXImporter)
//
// This module handles the Torch-MLIR integration pathway, converting
// torch dialect operations to ACHigh dialect operations.
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief PyTorch / Torch-MLIR to ACC MLIR importer. */

#ifndef ACC_FRONTEND_TORCHIMPORTER_H
#define ACC_FRONTEND_TORCHIMPORTER_H

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"
#include <string>

namespace mlir {
class ModuleOp;
} // namespace mlir

namespace acc {

/// Configuration for PyTorch model import.
struct TorchImportConfig {
  std::string pathway = "onnx";  ///< Import pathway: "torch-mlir" or "onnx".
  bool canonicalize = true;       ///< Run torch-mlir canonicalization before conversion.
  bool decompose = true;         ///< Decompose complex torch ops into simpler ones.
};

/// Imports PyTorch models into ACC MLIR (Torch-MLIR or ONNX pathway).
class TorchImporter {
public:
  /// \param context MLIR context for creating ACC operations.
  explicit TorchImporter(mlir::MLIRContext &context);

  /// Import a PyTorch ExportedProgram (serialized) to ACC MLIR.
  /// \param path Path to the serialized ExportedProgram (.pt2).
  /// \param config Import configuration.
  /// \return ACC MLIR module, or nullptr on failure.
  mlir::OwningOpRef<mlir::ModuleOp>
  importExportedProgram(const std::string &path,
                        const TorchImportConfig &config);

  /// Import a Torch-MLIR module (in-memory) and convert to ACC MLIR.
  /// Converts torch.aten.* operations to achigh.* operations.
  /// \param torchModule Source Torch-MLIR module.
  /// \param config Import configuration.
  /// \return ACC MLIR module, or nullptr on failure.
  mlir::OwningOpRef<mlir::ModuleOp>
  importFromTorchMLIR(mlir::ModuleOp torchModule,
                      const TorchImportConfig &config);

  /// Import via ONNX pathway (delegates to ONNXImporter).
  /// \param onnxPath Path to the ONNX model file.
  /// \return ACC MLIR module, or nullptr on failure.
  mlir::OwningOpRef<mlir::ModuleOp>
  importFromONNX(const std::string &onnxPath);

private:
  mlir::MLIRContext &context_;

  /// Convert torch.aten operations to ACHigh dialect.
  mlir::LogicalResult convertTorchOp(mlir::Operation *op);

  // Torch → ACHigh operation converters
  mlir::LogicalResult convertAtenConv2d(mlir::Operation *op);
  mlir::LogicalResult convertAtenLinear(mlir::Operation *op);
  mlir::LogicalResult convertAtenRelu(mlir::Operation *op);
  mlir::LogicalResult convertAtenMatmul(mlir::Operation *op);
  mlir::LogicalResult convertAtenBatchNorm(mlir::Operation *op);
  mlir::LogicalResult convertAtenSoftmax(mlir::Operation *op);
  mlir::LogicalResult convertAtenGelu(mlir::Operation *op);
  mlir::LogicalResult convertAtenMaxPool2d(mlir::Operation *op);
  mlir::LogicalResult convertAtenFlatten(mlir::Operation *op);
  mlir::LogicalResult convertAtenAdd(mlir::Operation *op);
  mlir::LogicalResult convertAtenMul(mlir::Operation *op);
};

} // namespace acc

#endif // ACC_FRONTEND_TORCHIMPORTER_H
