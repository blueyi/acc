//===- main.cpp - ACC JIT Runner -------------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// ac-runner compiles and executes MLIR or LLVM IR using JIT compilation.
// This is useful for testing and rapid prototyping without generating
// object files.
//
// Usage:
//   ac-runner input.mlir --entry-point=main
//   ac-runner input.mlir --entry-point=inference --args="..."
//
//===----------------------------------------------------------------------===//

#include "ACC/InitAllDialects.h"
#include "ACC/Support/Logger.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"

namespace cl = llvm::cl;

static cl::opt<std::string> inputFilename(cl::Positional,
                                           cl::desc("<input file>"),
                                           cl::Required);

static cl::opt<std::string> entryPoint("entry-point",
                                        cl::desc("Entry point function name"),
                                        cl::init("main"));

int main(int argc, char **argv) {
  llvm::InitLLVM initLLVM(argc, argv);
  cl::ParseCommandLineOptions(argc, argv, "ACC JIT Runner\n");

  // TODO: Implement JIT execution pipeline
  // 1. Parse input MLIR file
  // 2. Lower to LLVM Dialect
  // 3. Translate to LLVM IR
  // 4. Create JIT engine
  // 5. Execute the entry point function

  acc::Logger::instance().info("JIT runner not yet implemented");
  return 0;
}
