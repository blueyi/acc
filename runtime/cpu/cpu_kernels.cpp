//===- cpu_kernels.cpp - CPU Kernel Implementations ------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "cpu_kernels.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

extern "C" {

// ===========================================================================
// Matrix Multiplication
// ===========================================================================

void ac_cpu_matmul_f32(const float *A, const float *B, float *C,
                        int64_t M, int64_t N, int64_t K) {
  memset(C, 0, M * N * sizeof(float));
  // ikj loop order for better cache behavior on B
  for (int64_t i = 0; i < M; ++i) {
    for (int64_t k = 0; k < K; ++k) {
      float a_val = A[i * K + k];
      for (int64_t j = 0; j < N; ++j) {
        C[i * N + j] += a_val * B[k * N + j];
      }
    }
  }
}

void ac_cpu_matmul_tiled_f32(const float *A, const float *B, float *C,
                              int64_t M, int64_t N, int64_t K,
                              int64_t tile_size) {
  memset(C, 0, M * N * sizeof(float));
  for (int64_t i0 = 0; i0 < M; i0 += tile_size) {
    for (int64_t j0 = 0; j0 < N; j0 += tile_size) {
      for (int64_t k0 = 0; k0 < K; k0 += tile_size) {
        int64_t i_end = std::min(i0 + tile_size, M);
        int64_t j_end = std::min(j0 + tile_size, N);
        int64_t k_end = std::min(k0 + tile_size, K);
        for (int64_t i = i0; i < i_end; ++i) {
          for (int64_t k = k0; k < k_end; ++k) {
            float a_val = A[i * K + k];
            for (int64_t j = j0; j < j_end; ++j) {
              C[i * N + j] += a_val * B[k * N + j];
            }
          }
        }
      }
    }
  }
}

// ===========================================================================
// Convolution
// ===========================================================================

void ac_cpu_conv2d_naive_f32(const float *input, const float *filter,
                              float *output,
                              int64_t batch, int64_t in_channels,
                              int64_t in_h, int64_t in_w,
                              int64_t out_channels,
                              int64_t kernel_h, int64_t kernel_w,
                              int64_t stride_h, int64_t stride_w,
                              int64_t pad_h, int64_t pad_w) {
  int64_t out_h = (in_h + 2 * pad_h - kernel_h) / stride_h + 1;
  int64_t out_w = (in_w + 2 * pad_w - kernel_w) / stride_w + 1;
  memset(output, 0, batch * out_channels * out_h * out_w * sizeof(float));

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
                  float in_val =
                      input[((n * in_channels + ic) * in_h + ih) * in_w + iw];
                  float f_val =
                      filter[((oc * in_channels + ic) * kernel_h + kh) *
                                 kernel_w +
                             kw];
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

void ac_cpu_im2col_f32(const float *input, float *col_matrix,
                        int64_t N, int64_t C, int64_t H, int64_t W,
                        int64_t R, int64_t S,
                        int64_t stride_h, int64_t stride_w,
                        int64_t pad_h, int64_t pad_w,
                        int64_t H_out, int64_t W_out) {
  int64_t col_width = C * R * S;

#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif
  for (int64_t n = 0; n < N; ++n) {
    for (int64_t h_out = 0; h_out < H_out; ++h_out) {
      for (int64_t w_out = 0; w_out < W_out; ++w_out) {
        int64_t h_in = h_out * stride_h - pad_h;
        int64_t w_in = w_out * stride_w - pad_w;

        float *col_ptr =
            col_matrix +
            ((n * H_out + h_out) * W_out + w_out) * col_width;

        for (int64_t c = 0; c < C; ++c) {
          for (int64_t r = 0; r < R; ++r) {
            for (int64_t s = 0; s < S; ++s) {
              int64_t h = h_in + r;
              int64_t w = w_in + s;
              *col_ptr++ = (h >= 0 && h < H && w >= 0 && w < W)
                               ? input[((n * C + c) * H + h) * W + w]
                               : 0.0f;
            }
          }
        }
      }
    }
  }
}

void ac_cpu_conv2d_im2col_f32(const float *input, const float *weight,
                               const float *bias, float *output,
                               int64_t N, int64_t C, int64_t H, int64_t W,
                               int64_t K, int64_t R, int64_t S,
                               int64_t stride_h, int64_t stride_w,
                               int64_t pad_h, int64_t pad_w) {
  int64_t H_out = (H + 2 * pad_h - R) / stride_h + 1;
  int64_t W_out = (W + 2 * pad_w - S) / stride_w + 1;
  int64_t col_width = C * R * S;
  int64_t M_gemm = N * H_out * W_out;

  // 1. im2col: unfold input to matrix [M_gemm, col_width]
  std::vector<float> col(M_gemm * col_width);
  ac_cpu_im2col_f32(input, col.data(), N, C, H, W, R, S,
                     stride_h, stride_w, pad_h, pad_w, H_out, W_out);

  // 2. GEMM: output = col @ weight^T → [M_gemm, K]
  // weight layout: [K, C*R*S]
  ac_cpu_matmul_f32(col.data(), weight, output, M_gemm, K, col_width);

  // 3. Add bias (if provided)
  if (bias) {
    for (int64_t n = 0; n < N; ++n) {
      for (int64_t k = 0; k < K; ++k) {
        for (int64_t hw = 0; hw < H_out * W_out; ++hw) {
          output[(n * K + k) * H_out * W_out + hw] += bias[k];
        }
      }
    }
  }
}

// ===========================================================================
// Activation Functions
// ===========================================================================

void ac_cpu_relu_f32(const float *input, float *output, int64_t size) {
  for (int64_t i = 0; i < size; ++i) {
    output[i] = std::max(0.0f, input[i]);
  }
}

void ac_cpu_sigmoid_f32(const float *input, float *output, int64_t size) {
  for (int64_t i = 0; i < size; ++i) {
    output[i] = 1.0f / (1.0f + std::exp(-input[i]));
  }
}

void ac_cpu_tanh_f32(const float *input, float *output, int64_t size) {
  for (int64_t i = 0; i < size; ++i) {
    output[i] = std::tanh(input[i]);
  }
}

void ac_cpu_gelu_f32(const float *input, float *output, int64_t size) {
  // GELU(x) = 0.5 * x * (1 + tanh(sqrt(2/pi) * (x + 0.044715 * x^3)))
  const float sqrt_2_over_pi = 0.7978845608f;
  for (int64_t i = 0; i < size; ++i) {
    float x = input[i];
    float x3 = x * x * x;
    float inner = sqrt_2_over_pi * (x + 0.044715f * x3);
    output[i] = 0.5f * x * (1.0f + std::tanh(inner));
  }
}

// ===========================================================================
// Element-wise Operations
// ===========================================================================

void ac_cpu_add_f32(const float *a, const float *b, float *c, int64_t size) {
  for (int64_t i = 0; i < size; ++i) {
    c[i] = a[i] + b[i];
  }
}

void ac_cpu_mul_f32(const float *a, const float *b, float *c, int64_t size) {
  for (int64_t i = 0; i < size; ++i) {
    c[i] = a[i] * b[i];
  }
}

// ===========================================================================
// Normalization
// ===========================================================================

void ac_cpu_batch_norm_f32(const float *input, const float *scale,
                            const float *bias, const float *mean,
                            const float *variance, float *output,
                            int64_t N, int64_t C, int64_t H, int64_t W,
                            float epsilon) {
  int64_t spatial = H * W;
  for (int64_t n = 0; n < N; ++n) {
    for (int64_t c = 0; c < C; ++c) {
      float s = scale[c];
      float b = bias[c];
      float m = mean[c];
      float inv_std = 1.0f / std::sqrt(variance[c] + epsilon);
      for (int64_t hw = 0; hw < spatial; ++hw) {
        int64_t idx = (n * C + c) * spatial + hw;
        output[idx] = s * (input[idx] - m) * inv_std + b;
      }
    }
  }
}

// ===========================================================================
// Pooling
// ===========================================================================

void ac_cpu_maxpool2d_f32(const float *input, float *output,
                           int64_t N, int64_t C, int64_t H, int64_t W,
                           int64_t kernel_h, int64_t kernel_w,
                           int64_t stride_h, int64_t stride_w,
                           int64_t pad_h, int64_t pad_w) {
  int64_t H_out = (H + 2 * pad_h - kernel_h) / stride_h + 1;
  int64_t W_out = (W + 2 * pad_w - kernel_w) / stride_w + 1;

  for (int64_t n = 0; n < N; ++n) {
    for (int64_t c = 0; c < C; ++c) {
      for (int64_t oh = 0; oh < H_out; ++oh) {
        for (int64_t ow = 0; ow < W_out; ++ow) {
          float max_val = -1e30f;
          for (int64_t kh = 0; kh < kernel_h; ++kh) {
            for (int64_t kw = 0; kw < kernel_w; ++kw) {
              int64_t ih = oh * stride_h - pad_h + kh;
              int64_t iw = ow * stride_w - pad_w + kw;
              if (ih >= 0 && ih < H && iw >= 0 && iw < W) {
                float val = input[((n * C + c) * H + ih) * W + iw];
                max_val = std::max(max_val, val);
              }
            }
          }
          output[((n * C + c) * H_out + oh) * W_out + ow] = max_val;
        }
      }
    }
  }
}

void ac_cpu_avgpool2d_f32(const float *input, float *output,
                           int64_t N, int64_t C, int64_t H, int64_t W,
                           int64_t kernel_h, int64_t kernel_w,
                           int64_t stride_h, int64_t stride_w,
                           int64_t pad_h, int64_t pad_w) {
  int64_t H_out = (H + 2 * pad_h - kernel_h) / stride_h + 1;
  int64_t W_out = (W + 2 * pad_w - kernel_w) / stride_w + 1;

  for (int64_t n = 0; n < N; ++n) {
    for (int64_t c = 0; c < C; ++c) {
      for (int64_t oh = 0; oh < H_out; ++oh) {
        for (int64_t ow = 0; ow < W_out; ++ow) {
          float sum = 0.0f;
          int count = 0;
          for (int64_t kh = 0; kh < kernel_h; ++kh) {
            for (int64_t kw = 0; kw < kernel_w; ++kw) {
              int64_t ih = oh * stride_h - pad_h + kh;
              int64_t iw = ow * stride_w - pad_w + kw;
              if (ih >= 0 && ih < H && iw >= 0 && iw < W) {
                sum += input[((n * C + c) * H + ih) * W + iw];
                count++;
              }
            }
          }
          output[((n * C + c) * H_out + oh) * W_out + ow] =
              (count > 0) ? sum / count : 0.0f;
        }
      }
    }
  }
}

// ===========================================================================
// Softmax
// ===========================================================================

void ac_cpu_softmax_f32(const float *input, float *output,
                         int64_t outer_size, int64_t axis_size) {
  for (int64_t i = 0; i < outer_size; ++i) {
    const float *in_ptr = input + i * axis_size;
    float *out_ptr = output + i * axis_size;

    // Find max for numerical stability
    float max_val = in_ptr[0];
    for (int64_t j = 1; j < axis_size; ++j) {
      max_val = std::max(max_val, in_ptr[j]);
    }

    // Compute exp and sum
    float sum = 0.0f;
    for (int64_t j = 0; j < axis_size; ++j) {
      out_ptr[j] = std::exp(in_ptr[j] - max_val);
      sum += out_ptr[j];
    }

    // Normalize
    float inv_sum = 1.0f / sum;
    for (int64_t j = 0; j < axis_size; ++j) {
      out_ptr[j] *= inv_sum;
    }
  }
}

} // extern "C"
