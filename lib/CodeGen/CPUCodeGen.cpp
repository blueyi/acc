//===- CPUCodeGen.cpp - CPU Backend Code Generation -------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/CodeGen/CPUCodeGen.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Host.h"

namespace acc {

CodeGenConfig createHostCPUConfig() {
  CodeGenConfig config;
  config.targetTriple = llvm::sys::getDefaultTargetTriple();
  config.cpu = "generic";
  config.features = "";
  config.optLevel = 2;
  return config;
}

CodeGenConfig createCPUConfig(const std::string &triple,
                               const std::string &cpu,
                               const std::string &features) {
  CodeGenConfig config;
  config.targetTriple = triple;
  config.cpu = cpu;
  config.features = features;
  config.optLevel = 2;
  return config;
}

} // namespace acc
