//===- JITEngine.cpp - JIT Compilation Engine -------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/CodeGen/JITEngine.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/Support/Error.h"

namespace acc {

std::unique_ptr<JITEngine> JITEngine::create() {
  auto jitEngine = std::unique_ptr<JITEngine>(new JITEngine());

  // TODO: Create LLJIT instance
  // auto jit = llvm::orc::LLJITBuilder().create();

  return jitEngine;
}

bool JITEngine::addModule(std::unique_ptr<llvm::Module> module,
                           std::unique_ptr<llvm::LLVMContext> context) {
  // TODO: Add module to JIT
  return false;
}

void *JITEngine::lookup(const std::string &name) {
  // TODO: Look up symbol address
  return nullptr;
}

bool JITEngine::execute(const std::string &funcName) {
  // TODO: Execute the named function
  return false;
}

} // namespace acc
