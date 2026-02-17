//===- CodeGenDriver.h - Code Generation Driver -----------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// The CodeGen driver orchestrates the entire code generation process:
// LLVM IR → Target Machine Code → Object File / Executable.
//
//===----------------------------------------------------------------------===//

#ifndef ACC_CODEGEN_CODEGENDRIVER_H
#define ACC_CODEGEN_CODEGENDRIVER_H

#include "llvm/IR/Module.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>
#include <string>

namespace acc {

/// Configuration for code generation.
struct CodeGenConfig {
  std::string targetTriple;     // e.g., "x86_64-unknown-linux-gnu"
  std::string cpu;              // e.g., "generic", "skylake"
  std::string features;         // e.g., "+avx2,+fma"
  std::string outputFile;
  bool emitAssembly = false;    // true = .s, false = .o
  bool emitLLVMIR = false;
  int optLevel = 2;             // 0-3
};

class CodeGenDriver {
public:
  explicit CodeGenDriver(const CodeGenConfig &config);

  /// Perform code generation on the given LLVM IR module.
  /// Returns true on success.
  bool generate(llvm::Module &module);

  /// Get the target machine (for JIT or further processing).
  llvm::TargetMachine *getTargetMachine() const { return targetMachine_.get(); }

private:
  CodeGenConfig config_;
  std::unique_ptr<llvm::TargetMachine> targetMachine_;

  bool initTargetMachine();
  bool emitObjectFile(llvm::Module &module);
  bool emitAssembly(llvm::Module &module);
};

} // namespace acc

#endif // ACC_CODEGEN_CODEGENDRIVER_H
