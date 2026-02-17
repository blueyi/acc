//===- bench_conv2d.cpp - Conv2D Performance Benchmark ---------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "benchmark.h"
#include "../../runtime/cpu/cpu_kernels.h"
#include <cstdlib>
#include <vector>

using namespace acc::benchmark;

void benchmarkConv2D(int64_t N, int64_t C, int64_t H, int64_t W,
                     int64_t K, int64_t R, int64_t S,
                     int64_t stride, int64_t pad) {
  int64_t H_out = (H + 2 * pad - R) / stride + 1;
  int64_t W_out = (W + 2 * pad - S) / stride + 1;

  std::vector<float> input(N * C * H * W);
  std::vector<float> weight(K * C * R * S);
  std::vector<float> output(N * K * H_out * W_out);

  for (auto &v : input) v = static_cast<float>(rand()) / RAND_MAX;
  for (auto &v : weight) v = static_cast<float>(rand()) / RAND_MAX;

  double flops = conv2dFlops(N, C, K, H_out, W_out, R, S);

  // Benchmark naive conv2d
  char name[128];
  snprintf(name, sizeof(name), "Conv2D[%ldx%ldx%ldx%ld,k%ld] naive",
           N, C, H, W, R);
  auto resultNaive = OpBenchmark::run(
      name,
      [&]() {
        ac_cpu_conv2d_naive_f32(input.data(), weight.data(), output.data(),
                                 N, C, H, W, K, R, S, stride, stride, pad, pad);
      },
      2, 10, flops, 0);
  OpBenchmark::printResult(resultNaive);

  // Benchmark im2col+GEMM conv2d
  snprintf(name, sizeof(name), "Conv2D[%ldx%ldx%ldx%ld,k%ld] im2col",
           N, C, H, W, R);
  auto resultIm2col = OpBenchmark::run(
      name,
      [&]() {
        ac_cpu_conv2d_im2col_f32(input.data(), weight.data(), nullptr,
                                  output.data(), N, C, H, W, K, R, S,
                                  stride, stride, pad, pad);
      },
      2, 10, flops, 0);
  OpBenchmark::printResult(resultIm2col);
}

int main() {
  printf("=== ACC Conv2D Benchmark ===\n\n");

  // Small conv
  benchmarkConv2D(1, 3, 32, 32, 64, 3, 3, 1, 1);
  // Medium conv (ResNet style)
  benchmarkConv2D(1, 64, 56, 56, 64, 3, 3, 1, 1);
  // Large conv
  benchmarkConv2D(1, 3, 224, 224, 64, 7, 7, 2, 3);

  return 0;
}
