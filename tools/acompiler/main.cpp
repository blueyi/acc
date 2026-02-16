//===- main.cpp - ACompiler Main Driver -------------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Main entry point for the ACompiler compiler driver. This tool orchestrates
// the complete compilation pipeline:
//   1. Frontend: Parse source (DSL or ONNX) → AST → MLIR (ACHigh)
//   2. MLIR Optimization: Graph-level passes on ACHigh dialect
//   3. Lowering: ACHigh → ACMid → Linalg → LLVM Dialect
//   4. LLVM IR Optimization: Custom + standard LLVM passes
//   5. CodeGen: Target code generation (CPU / GPU / NPU)
//
// Usage:
//   acompiler input.ac -o output           # Compile DSL source
//   acompiler model.onnx -o output         # Compile ONNX model
//   acompiler input.mlir -o output         # Compile from MLIR
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Support/Logger.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

namespace cl = llvm::cl;

static cl::opt<std::string> inputFilename(cl::Positional,
                                           cl::desc("<input file>"),
                                           cl::Required);

static cl::opt<std::string> outputFilename("o", cl::desc("Output filename"),
                                            cl::value_desc("filename"),
                                            cl::init("-"));

static cl::opt<int> optLevel("O", cl::desc("Optimization level (0-3)"),
                              cl::init(2));

static cl::opt<std::string> targetStr("target",
                                       cl::desc("Target triple"),
                                       cl::init(""));

static cl::opt<bool> emitMLIR("emit-mlir",
                                cl::desc("Emit MLIR output"),
                                cl::init(false));

static cl::opt<bool> emitLLVM("emit-llvm",
                                cl::desc("Emit LLVM IR output"),
                                cl::init(false));

static cl::opt<bool> emitAsm("S",
                               cl::desc("Emit assembly output"),
                               cl::init(false));

int main(int argc, char **argv) {
  llvm::InitLLVM initLLVM(argc, argv);
  cl::ParseCommandLineOptions(argc, argv, "ACompiler - AI Compiler\n");

  acompiler::Logger::instance().info("ACompiler v0.1.0");
  acompiler::Logger::instance().info("Input: " + inputFilename);

  // TODO: Implement the full compilation pipeline
  // 1. Determine input type (DSL / ONNX / MLIR)
  // 2. Run frontend to produce MLIR
  // 3. Run MLIR optimization passes
  // 4. Run lowering pipeline
  // 5. Convert to LLVM IR
  // 6. Run LLVM IR optimizations
  // 7. Run CodeGen

  llvm::errs() << "ACompiler: compilation pipeline not yet implemented\n";
  return 0;
}
