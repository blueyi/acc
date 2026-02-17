//===- CodeGenDriver.cpp - Code Generation Driver ---------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/CodeGen/CodeGenDriver.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetOptions.h"

namespace acc {

CodeGenDriver::CodeGenDriver(const CodeGenConfig &config) : config_(config) {}

bool CodeGenDriver::generate(llvm::Module &module) {
  if (!initTargetMachine())
    return false;

  module.setTargetTriple(config_.targetTriple);
  module.setDataLayout(targetMachine_->createDataLayout());

  if (config_.emitAssembly)
    return emitAssembly(module);
  return emitObjectFile(module);
}

bool CodeGenDriver::initTargetMachine() {
  // TODO: Initialize target machine based on config
  // 1. Look up target by triple
  // 2. Create TargetMachine with specified CPU and features
  // 3. Set optimization level
  return false;
}

bool CodeGenDriver::emitObjectFile(llvm::Module &module) {
  // TODO: Emit object file (.o)
  return false;
}

bool CodeGenDriver::emitAssembly(llvm::Module &module) {
  // TODO: Emit assembly file (.s)
  return false;
}

} // namespace acc
