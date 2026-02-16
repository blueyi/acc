//===- ACompilerOpt.cpp - ACompiler MLIR optimization tool --------------===//
//
// acompiler-opt: MLIR 级优化工具，类似 mlir-opt
// 加载 AC Dialect 并执行 Pass Pipeline
// 第一阶段：占位实现，后续集成 MlirOptMain 完整功能
//
//===----------------------------------------------------------------------===//

#include "mlir/IR/AsmState.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/InitAllDialects.h"
#include "mlir/Support/LogicalResult.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

#include "ACDialect.h"

static llvm::cl::opt<std::string> InputFilename(llvm::cl::Positional,
  llvm::cl::desc("<input .mlir file>"),
  llvm::cl::init("-"));

int main(int argc, char **argv) {
  llvm::InitLLVM y(argc, argv);
  llvm::cl::ParseCommandLineOptions(argc, argv,
    "acompiler-opt - ACompiler MLIR optimization driver\n");

  mlir::DialectRegistry registry;
  mlir::registerAllDialects(registry);
  registry.insert<mlir::ac::ACDialect>();

  mlir::MLIRContext context(registry);

  llvm::outs() << "acompiler-opt (placeholder - full pipeline not yet implemented)\n";
  llvm::outs() << "  Input: " << InputFilename << "\n";
  llvm::outs() << "  AC Dialect loaded.\n";

  return 0;
}
