//===- cpu_kernels.cpp - CPU Kernel Implementations ------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "cpu_kernels.h"
#include <algorithm>
#include <cstring>

extern "C" {

void ac_cpu_matmul_f32(const float *A, const float *B, float *C,
                        int64_t M, int64_t N, int64_t K) {
  memset(C, 0, M * N * sizeof(float));
  for (int64_t i = 0; i < M; ++i) {
    for (int64_t k = 0; k < K; ++k) {
      float a_val = A[i * K + k];
      for (int64_t j = 0; j < N; ++j) {
        C[i * N + j] += a_val * B[k * N + j];
      }
    }
  }
}

void ac_cpu_conv2d_nchw_f32(const float *input, const float *filter,
                              float *output,
                              int64_t batch, int64_t in_channels,
                              int64_t in_h, int64_t in_w,
                              int64_t out_channels,
                              int64_t kernel_h, int64_t kernel_w,
                              int64_t stride_h, int64_t stride_w,
                              int64_t pad_h, int64_t pad_w) {
  int64_t out_h = (in_h + 2 * pad_h - kernel_h) / stride_h + 1;
  int64_t out_w = (in_w + 2 * pad_w - kernel_w) / stride_w + 1;

  memset(output, 0,
         batch * out_channels * out_h * out_w * sizeof(float));

  for (int64_t n = 0; n < batch; ++n) {
    for (int64_t oc = 0; oc < out_channels; ++oc) {
      for (int64_t oh = 0; oh < out_h; ++oh) {
        for (int64_t ow = 0; ow < out_w; ++ow) {
          float sum = 0.0f;
          for (int64_t ic = 0; ic < in_channels; ++ic) {
            for (int64_t kh = 0; kh < kernel_h; ++kh) {
              for (int64_t kw = 0; kw < kernel_w; ++kw) {
                int64_t ih = oh * stride_h - pad_h + kh;
                int64_t iw = ow * stride_w - pad_w + kw;
                if (ih >= 0 && ih < in_h && iw >= 0 && iw < in_w) {
                  float in_val = input[((n * in_channels + ic) * in_h + ih) *
                                       in_w + iw];
                  float f_val = filter[((oc * in_channels + ic) * kernel_h +
                                        kh) * kernel_w + kw];
                  sum += in_val * f_val;
                }
              }
            }
          }
          output[((n * out_channels + oc) * out_h + oh) * out_w + ow] = sum;
        }
      }
    }
  }
}

void ac_cpu_relu_f32(const float *input, float *output, int64_t size) {
  for (int64_t i = 0; i < size; ++i) {
    output[i] = std::max(0.0f, input[i]);
  }
}

void ac_cpu_add_f32(const float *a, const float *b, float *c, int64_t size) {
  for (int64_t i = 0; i < size; ++i) {
    c[i] = a[i] + b[i];
  }
}

} // extern "C"
