#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

static llvm::cl::opt<std::string> InputPath(
    llvm::cl::Positional, llvm::cl::desc("<input>"), llvm::cl::init("-"));

int main(int argc, char **argv) {
  llvm::InitLLVM y(argc, argv);
  llvm::cl::ParseCommandLineOptions(argc, argv, "ACompiler driver (WIP)\n");

  llvm::outs() << "acompilerc: TODO (CPU-only pipeline will land in later milestones)\n";
  llvm::outs() << "input: " << InputPath << "\n";
  return 0;
}

