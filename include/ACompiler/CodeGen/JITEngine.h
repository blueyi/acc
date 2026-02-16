//===- JITEngine.h - JIT Compilation Engine ----------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// LLVM ORC JIT-based execution engine for ACompiler. Allows compiling and
// running AI models at runtime without generating object files.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_CODEGEN_JITENGINE_H
#define ACOMPILER_CODEGEN_JITENGINE_H

#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/IR/Module.h"
#include <memory>
#include <string>

namespace acompiler {

class JITEngine {
public:
  /// Create a JIT engine instance.
  static std::unique_ptr<JITEngine> create();

  /// Add an LLVM module to the JIT.
  bool addModule(std::unique_ptr<llvm::Module> module,
                 std::unique_ptr<llvm::LLVMContext> context);

  /// Look up a symbol (function) by name and return its address.
  void *lookup(const std::string &name);

  /// Execute a function with no arguments and no return value.
  bool execute(const std::string &funcName);

private:
  std::unique_ptr<llvm::orc::LLJIT> jit_;

  JITEngine() = default;
};

} // namespace acompiler

#endif // ACOMPILER_CODEGEN_JITENGINE_H
