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

/** \file
 * \brief CPU backend code generation (host and target configs). */

#ifndef ACC_CODEGEN_CPUCODEGEN_H
#define ACC_CODEGEN_CPUCODEGEN_H

#include "ACC/CodeGen/CodeGenDriver.h"

namespace acc {

/// Create a CodeGenConfig for the host CPU with auto-detected features.
/// \return Configuration for the current host (triple, CPU, features).
CodeGenConfig createHostCPUConfig();

/// Create a CodeGenConfig for a specific CPU target.
/// \param triple LLVM target triple (e.g. "x86_64-unknown-linux-gnu").
/// \param cpu CPU name (e.g. "skylake", "generic").
/// \param features Target feature string (e.g. "+avx2,+fma").
/// \return Code generation configuration.
CodeGenConfig createCPUConfig(const std::string &triple,
                               const std::string &cpu,
                               const std::string &features);

} // namespace acc

#endif // ACC_CODEGEN_CPUCODEGEN_H
