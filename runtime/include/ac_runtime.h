//===- ac_runtime.h - ACompiler Runtime Public API --------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Public API for the ACompiler runtime library. This API is called by
// generated code at inference time.
//
//===----------------------------------------------------------------------===//

#ifndef AC_RUNTIME_H
#define AC_RUNTIME_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Allocate aligned memory for tensor data.
void *ac_alloc(size_t size, size_t alignment);

/// Free memory allocated by ac_alloc.
void ac_free(void *ptr);

/// Print a tensor (for debugging).
void ac_print_tensor_f32(float *data, int64_t *shape, int64_t rank);

/// Get the current time in milliseconds (for profiling).
double ac_get_time_ms();

#ifdef __cplusplus
}
#endif

#endif // AC_RUNTIME_H
