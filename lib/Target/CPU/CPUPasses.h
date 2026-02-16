//===- CPUPasses.h - CPU backend passes -------------------------*- C++ -*-===//
//
// CPU 后端 Pass 声明
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_TARGET_CPU_CPUPASSES_H
#define ACOMPILER_TARGET_CPU_CPUPASSES_H

namespace mlir {
class PassRegistration;
}

namespace acompiler {
namespace cpu {

/// 注册 CPU 相关 Pass
void registerCPUPasses();

} // namespace cpu
} // namespace acompiler

#endif // ACOMPILER_TARGET_CPU_CPUPASSES_H
