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

/** \file
 * \brief Code generation driver (LLVM IR to object/assembly). */

#ifndef ACC_CODEGEN_CODEGENDRIVER_H
#define ACC_CODEGEN_CODEGENDRIVER_H

#include "llvm/IR/Module.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>
#include <string>

namespace acc {

/// Configuration for code generation (target, CPU, output, optimization).
struct CodeGenConfig {
  std::string targetTriple;   ///< LLVM target triple, e.g. "x86_64-unknown-linux-gnu".
  std::string cpu;            ///< CPU name, e.g. "generic", "skylake".
  std::string features;       ///< Target features, e.g. "+avx2,+fma".
  std::string outputFile;     ///< Output file path.
  bool emitAssembly = false;  ///< If true emit .s, else .o.
  bool emitLLVMIR = false;    ///< If true emit LLVM IR instead of machine code.
  int optLevel = 2;           ///< Optimization level 0–3.
};

/// Drives code generation from LLVM IR to object file or assembly.
class CodeGenDriver {
public:
  /// \param config Code generation configuration.
  explicit CodeGenDriver(const CodeGenConfig &config);

  /// Perform code generation on the given LLVM IR module.
  /// \param module The LLVM module to compile.
  /// \return true on success.
  bool generate(llvm::Module &module);

  /// Get the target machine (for JIT or further processing).
  /// \return The target machine, or nullptr if not initialized.
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
