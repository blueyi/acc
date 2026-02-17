//===- cpu_runtime.h - CPU Runtime Interface --------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef AC_RUNTIME_CPU_H
#define AC_RUNTIME_CPU_H

#include "ac_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Initialize the CPU runtime.
void ac_cpu_init();

/// Finalize and clean up the CPU runtime.
void ac_cpu_finalize();

#ifdef __cplusplus
}
#endif

#endif // AC_RUNTIME_CPU_H
