//===- Passes.h - ACompiler LLVM IR Optimization Passes ---------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Registration and creation functions for custom LLVM IR optimization passes.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_IROPTIMIZER_PASSES_H
#define ACOMPILER_IROPTIMIZER_PASSES_H

#include "llvm/IR/PassManager.h"

namespace acompiler {

/// Custom loop optimization pass for LLVM IR.
struct CustomLoopOptPass : public llvm::PassInfoMixin<CustomLoopOptPass> {
  llvm::PreservedAnalyses run(llvm::Function &F,
                               llvm::FunctionAnalysisManager &AM);
};

/// Custom vectorization pass for LLVM IR.
struct CustomVectorizePass : public llvm::PassInfoMixin<CustomVectorizePass> {
  llvm::PreservedAnalyses run(llvm::Function &F,
                               llvm::FunctionAnalysisManager &AM);
};

/// Register all custom LLVM IR passes.
void registerLLVMIRPasses(llvm::PassBuilder &PB);

/// Build the default ACompiler LLVM IR optimization pipeline.
void buildACOptimizationPipeline(llvm::ModulePassManager &MPM,
                                  llvm::OptimizationLevel OptLevel);

} // namespace acompiler

#endif // ACOMPILER_IROPTIMIZER_PASSES_H
