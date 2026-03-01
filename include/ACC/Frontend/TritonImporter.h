//===- TritonImporter.h - Triton IR to ACC MLIR Importer --------*- C++ -*-===//
//
// Part of the ACC Project (AI Compiler Core)
//
//===----------------------------------------------------------------------===//
//
// Imports OpenAI Triton IR (TTIR) and converts it to ACC MLIR representation.
// This enables the flow: Triton DSL → Triton IR → ACC MLIR → CodeGen.
//
// Supported Triton operations:
//   tl.load    → memref.load (with mask → scf.if guard)
//   tl.store   → memref.store (with mask)
//   tl.dot     → achigh.matmul (block-level matrix multiply)
//   tl.reduce  → achigh reduction operations
//   tl.arange  → arith.constant + affine indexing
//   tl.zeros   → arith.constant (zero-filled tensor)
//   tl.maximum → arith.maxf (element-wise max, used for ReLU)
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief Triton IR (TTIR) to ACC MLIR importer. */

#ifndef ACC_FRONTEND_TRITONIMPORTER_H
#define ACC_FRONTEND_TRITONIMPORTER_H

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace mlir {
class ModuleOp;
class Operation;
} // namespace mlir

namespace acc {

/// Configuration for Triton kernel import.
struct TritonImportConfig {
  /// Block size constants (e.g., BLOCK_M=32, BLOCK_N=32, BLOCK_K=32).
  std::unordered_map<std::string, int64_t> constexprArgs;
  /// Number of warps (default: 4).
  int numWarps = 4;
  /// Number of stages for software pipelining (default: 2).
  int numStages = 2;
};

/// Imports Triton IR (TTIR MLIR dialect) into ACC MLIR.
///
/// The importer works in two modes:
/// 1. From serialized Triton IR file (.ttir)
/// 2. From Triton MLIR module (in-memory, for Python binding)
class TritonImporter {
public:
  /// \param context MLIR context for creating ACC operations.
  explicit TritonImporter(mlir::MLIRContext &context);

  /// Import a Triton IR file (.ttir) and convert to ACC MLIR.
  /// \param path Path to the serialized Triton IR file.
  /// \param config Block size and other constexpr parameters.
  /// \return ACC MLIR module, or nullptr on failure.
  mlir::OwningOpRef<mlir::ModuleOp>
  importFromFile(const std::string &path, const TritonImportConfig &config);

  /// Import a Triton MLIR module (in-memory) and convert to ACC MLIR.
  /// \param tritonModule The source Triton MLIR module.
  /// \param config Block size and other constexpr parameters.
  /// \return ACC MLIR module, or nullptr on failure.
  mlir::OwningOpRef<mlir::ModuleOp>
  importFromModule(mlir::ModuleOp tritonModule,
                   const TritonImportConfig &config);

private:
  mlir::MLIRContext &context_;

  /// Convert a single Triton operation to ACC MLIR operations.
  mlir::LogicalResult convertOperation(mlir::Operation *op);

  /// Map Triton block program semantics to ACC loop structure.
  mlir::LogicalResult convertBlockProgram(mlir::Operation *kernelFunc);

  // Individual operation converters
  mlir::LogicalResult convertTritonLoad(mlir::Operation *op);
  mlir::LogicalResult convertTritonStore(mlir::Operation *op);
  mlir::LogicalResult convertTritonDot(mlir::Operation *op);
  mlir::LogicalResult convertTritonReduce(mlir::Operation *op);
  mlir::LogicalResult convertTritonMakeRange(mlir::Operation *op);
  mlir::LogicalResult convertTritonSplat(mlir::Operation *op);
  mlir::LogicalResult convertTritonBroadcast(mlir::Operation *op);
};

} // namespace acc

#endif // ACC_FRONTEND_TRITONIMPORTER_H
