//===- TorchImporter.cpp - PyTorch Model to ACC MLIR Import -----*- C++ -*-===//
//
// Part of the ACC Project (AI Compiler Core)
//
//===----------------------------------------------------------------------===//

#include "ACC/Frontend/TorchImporter.h"
#include "ACC/Frontend/ONNXImporter.h"
#include "ACC/Dialect/ACHigh/ACHighDialect.h"
#include "ACC/Dialect/ACHigh/ACHighOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinOps.h"

namespace acc {

TorchImporter::TorchImporter(mlir::MLIRContext &context)
    : context_(context) {}

mlir::OwningOpRef<mlir::ModuleOp>
TorchImporter::importExportedProgram(const std::string &path,
                                      const TorchImportConfig &config) {
  // TODO: Load PyTorch ExportedProgram (.pt2)
  // 1. Deserialize the ExportedProgram
  // 2. Convert FX Graph to Torch-MLIR
  // 3. Call importFromTorchMLIR
  return nullptr;
}

mlir::OwningOpRef<mlir::ModuleOp>
TorchImporter::importFromTorchMLIR(mlir::ModuleOp torchModule,
                                    const TorchImportConfig &config) {
  mlir::OpBuilder builder(&context_);
  auto accModule = mlir::ModuleOp::create(builder.getUnknownLoc());

  // TODO: Convert torch dialect ops to ACHigh dialect ops
  // Mapping:
  //   torch.aten.conv2d        → achigh.conv2d
  //   torch.aten.linear        → achigh.dense
  //   torch.aten.relu          → achigh.relu
  //   torch.aten.gelu          → achigh.gelu
  //   torch.aten.matmul        → achigh.matmul
  //   torch.aten.batch_norm    → achigh.batch_norm
  //   torch.aten.softmax       → achigh.softmax
  //   torch.aten.max_pool2d    → achigh.maxpool2d
  //   torch.aten.flatten       → achigh.flatten
  //   torch.aten.add.Tensor    → achigh.add
  //   torch.aten.mul.Tensor    → achigh.mul
  //   torch.aten.sigmoid       → achigh.sigmoid
  //   torch.aten.tanh          → achigh.tanh
  //   torch.aten.transpose     → achigh.transpose

  return accModule;
}

mlir::OwningOpRef<mlir::ModuleOp>
TorchImporter::importFromONNX(const std::string &onnxPath) {
  return importONNXModel(context_, onnxPath);
}

} // namespace acc
