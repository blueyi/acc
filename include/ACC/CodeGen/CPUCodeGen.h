//===- CPUCodeGen.h - CPU Backend Code Generation ---------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// CPU-specific code generation. Handles x86-64 and AArch64 targets with
// SIMD vectorization support (SSE/AVX/NEON/SVE).
//
//===----------------------------------------------------------------------===//

#ifndef ACC_CODEGEN_CPUCODEGEN_H
#define ACC_CODEGEN_CPUCODEGEN_H

#include "ACC/CodeGen/CodeGenDriver.h"

namespace acc {

/// Create a CodeGenConfig for the host CPU with auto-detected features.
CodeGenConfig createHostCPUConfig();

/// Create a CodeGenConfig for a specific CPU target.
CodeGenConfig createCPUConfig(const std::string &triple,
                               const std::string &cpu,
                               const std::string &features);

} // namespace acc

#endif // ACC_CODEGEN_CPUCODEGEN_H
