//===- ONNXImportRegistration.cpp - Register --import-onnx for ac-translate -*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Frontend/ONNXImporter.h"
#include "ACC/InitAllDialects.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Operation.h"
#include "mlir/IR/OwningOpRef.h"
#include "mlir/Tools/mlir-translate/Translation.h"
#include "llvm/Support/SourceMgr.h"

using namespace mlir;

namespace {

static TranslateToMLIRRegistration Unused(
    "import-onnx",
    "Import ONNX model file to ACC MLIR (ACHigh dialect)",
    [](const std::shared_ptr<llvm::SourceMgr> &sourceMgr, MLIRContext *context)
        -> OwningOpRef<Operation *> {
      if (!sourceMgr || sourceMgr->getNumBuffers() == 0)
        return OwningOpRef<Operation *>();
      unsigned mainID = sourceMgr->getMainFileID();
      llvm::StringRef path =
          sourceMgr->getBufferInfo(mainID).Buffer->getBufferIdentifier();
      auto module = acc::importONNXModel(*context, path.str());
      if (!module)
        return OwningOpRef<Operation *>();
      return OwningOpRef<Operation *>(module.release());
    },
    [](DialectRegistry &registry) { acc::registerAllDialects(registry); });

} // namespace
