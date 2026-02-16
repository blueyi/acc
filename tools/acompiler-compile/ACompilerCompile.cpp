//===- ACompilerCompile.cpp - ACompiler 端到端编译工具 -----------------===//
//
// acompiler-compile: 从输入到可执行文件/目标码的端到端编译
// 第一阶段：占位实现，后续补充完整 Pipeline
//
//===----------------------------------------------------------------------===//

#include "mlir/IR/DialectRegistry.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/InitAllDialects.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

#include "ACDialect.h"

static llvm::cl::opt<std::string> InputFilename(llvm::cl::Positional,
  llvm::cl::desc("<input file>"),
  llvm::cl::init("-"));

static llvm::cl::opt<std::string> OutputFilename("o",
  llvm::cl::desc("Output filename"),
  llvm::cl::value_desc("filename"),
  llvm::cl::init("-"));

int main(int argc, char **argv) {
  llvm::InitLLVM y(argc, argv);
  llvm::cl::ParseCommandLineOptions(argc, argv,
    "ACompiler - 端到端 AI 编译器\n");

  mlir::DialectRegistry registry;
  mlir::registerAllDialects(registry);
  registry.insert<mlir::ac::ACDialect>();

  mlir::MLIRContext context(registry);

  llvm::outs() << "ACompiler compile (placeholder - pipeline not yet implemented)\n";
  llvm::outs() << "  Input:  " << InputFilename << "\n";
  llvm::outs() << "  Output: " << OutputFilename << "\n";

  return 0;
}
