//===- cpu_kernels.h - CPU Kernel Declarations -----------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Hand-optimized CPU kernels for common AI operations. Includes both naive
// reference implementations and optimized versions (im2col+GEMM, SIMD).
//
//===----------------------------------------------------------------------===//

#ifndef AC_RUNTIME_CPU_KERNELS_H
#define AC_RUNTIME_CPU_KERNELS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ===========================================================================
// Matrix Multiplication
// ===========================================================================

/// Naive matrix multiplication: C = A * B
/// A: [M, K], B: [K, N], C: [M, N]
void ac_cpu_matmul_f32(const float *A, const float *B, float *C,
                        int64_t M, int64_t N, int64_t K);

/// Tiled matrix multiplication (cache-friendly)
void ac_cpu_matmul_tiled_f32(const float *A, const float *B, float *C,
                              int64_t M, int64_t N, int64_t K,
                              int64_t tile_size);

// ===========================================================================
// Convolution
// ===========================================================================

/// Naive 2D convolution (NCHW format) — reference implementation
void ac_cpu_conv2d_naive_f32(const float *input, const float *filter,
                              float *output,
                              int64_t batch, int64_t in_channels,
                              int64_t in_h, int64_t in_w,
                              int64_t out_channels,
                              int64_t kernel_h, int64_t kernel_w,
                              int64_t stride_h, int64_t stride_w,
                              int64_t pad_h, int64_t pad_w);

/// im2col transform: unfold input patches into a matrix for GEMM-based conv
/// Output: col_matrix of shape [N*H_out*W_out, C*R*S]
void ac_cpu_im2col_f32(const float *input, float *col_matrix,
                        int64_t N, int64_t C, int64_t H, int64_t W,
                        int64_t R, int64_t S,
                        int64_t stride_h, int64_t stride_w,
                        int64_t pad_h, int64_t pad_w,
                        int64_t H_out, int64_t W_out);

/// High-performance 2D convolution using im2col + GEMM
/// Converts convolution to matrix multiplication for better SIMD utilization.
void ac_cpu_conv2d_im2col_f32(const float *input, const float *weight,
                               const float *bias, float *output,
                               int64_t N, int64_t C, int64_t H, int64_t W,
                               int64_t K, int64_t R, int64_t S,
                               int64_t stride_h, int64_t stride_w,
                               int64_t pad_h, int64_t pad_w);

// ===========================================================================
// Activation Functions
// ===========================================================================

void ac_cpu_relu_f32(const float *input, float *output, int64_t size);
void ac_cpu_sigmoid_f32(const float *input, float *output, int64_t size);
void ac_cpu_tanh_f32(const float *input, float *output, int64_t size);
void ac_cpu_gelu_f32(const float *input, float *output, int64_t size);

// ===========================================================================
// Element-wise Operations
// ===========================================================================

void ac_cpu_add_f32(const float *a, const float *b, float *c, int64_t size);
void ac_cpu_mul_f32(const float *a, const float *b, float *c, int64_t size);

// ===========================================================================
// Normalization
// ===========================================================================

/// Batch normalization: output = scale * (input - mean) / sqrt(var + eps) + bias
void ac_cpu_batch_norm_f32(const float *input, const float *scale,
                            const float *bias, const float *mean,
                            const float *variance, float *output,
                            int64_t N, int64_t C, int64_t H, int64_t W,
                            float epsilon);

// ===========================================================================
// Pooling
// ===========================================================================

void ac_cpu_maxpool2d_f32(const float *input, float *output,
                           int64_t N, int64_t C, int64_t H, int64_t W,
                           int64_t kernel_h, int64_t kernel_w,
                           int64_t stride_h, int64_t stride_w,
                           int64_t pad_h, int64_t pad_w);

void ac_cpu_avgpool2d_f32(const float *input, float *output,
                           int64_t N, int64_t C, int64_t H, int64_t W,
                           int64_t kernel_h, int64_t kernel_w,
                           int64_t stride_h, int64_t stride_w,
                           int64_t pad_h, int64_t pad_w);

// ===========================================================================
// Softmax
// ===========================================================================

void ac_cpu_softmax_f32(const float *input, float *output,
                         int64_t outer_size, int64_t axis_size);

#ifdef __cplusplus
}
#endif

#endif // AC_RUNTIME_CPU_KERNELS_H
