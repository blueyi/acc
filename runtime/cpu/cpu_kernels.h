//===- cpu_kernels.h - CPU Kernel Declarations -----------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Hand-optimized CPU kernels for common AI operations. These serve as
// fallback implementations and reference for compiler-generated code.
//
//===----------------------------------------------------------------------===//

#ifndef AC_RUNTIME_CPU_KERNELS_H
#define AC_RUNTIME_CPU_KERNELS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Naive matrix multiplication: C = A * B
/// A: [M, K], B: [K, N], C: [M, N]
void ac_cpu_matmul_f32(const float *A, const float *B, float *C,
                        int64_t M, int64_t N, int64_t K);

/// Naive 2D convolution (NCHW format)
void ac_cpu_conv2d_nchw_f32(const float *input, const float *filter,
                              float *output,
                              int64_t batch, int64_t in_channels,
                              int64_t in_h, int64_t in_w,
                              int64_t out_channels,
                              int64_t kernel_h, int64_t kernel_w,
                              int64_t stride_h, int64_t stride_w,
                              int64_t pad_h, int64_t pad_w);

/// Element-wise ReLU
void ac_cpu_relu_f32(const float *input, float *output, int64_t size);

/// Element-wise addition
void ac_cpu_add_f32(const float *a, const float *b, float *c, int64_t size);

#ifdef __cplusplus
}
#endif

#endif // AC_RUNTIME_CPU_KERNELS_H
