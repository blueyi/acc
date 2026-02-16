#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* acompiler_runtime_alloc(size_t size_in_bytes);
void acompiler_runtime_free(void* ptr);

void acompiler_runtime_relu_f32(const float* input, float* output, size_t n);

#ifdef __cplusplus
}
#endif
