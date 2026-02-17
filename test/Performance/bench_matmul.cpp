//===- bench_matmul.cpp - MatMul Performance Benchmark ---------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "benchmark.h"
#include "../../runtime/cpu/cpu_kernels.h"
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace acc::benchmark;

void benchmarkMatMul(int64_t M, int64_t N, int64_t K) {
  std::vector<float> A(M * K);
  std::vector<float> B(K * N);
  std::vector<float> C(M * N);

  // Initialize with random values
  for (auto &v : A) v = static_cast<float>(rand()) / RAND_MAX;
  for (auto &v : B) v = static_cast<float>(rand()) / RAND_MAX;

  double flops = matmulFlops(M, N, K);
  double bytes = (M * K + K * N + M * N) * sizeof(float);

  // Benchmark naive matmul
  char name[128];
  snprintf(name, sizeof(name), "MatMul[%ld,%ld,%ld] naive", M, N, K);
  auto result = OpBenchmark::run(
      name, [&]() { ac_cpu_matmul_f32(A.data(), B.data(), C.data(), M, N, K); },
      5, 50, flops, bytes);
  OpBenchmark::printResult(result);

  // Benchmark tiled matmul
  snprintf(name, sizeof(name), "MatMul[%ld,%ld,%ld] tiled", M, N, K);
  auto resultTiled = OpBenchmark::run(
      name,
      [&]() {
        ac_cpu_matmul_tiled_f32(A.data(), B.data(), C.data(), M, N, K, 32);
      },
      5, 50, flops, bytes);
  OpBenchmark::printResult(resultTiled);
}

int main() {
  printf("=== ACC MatMul Benchmark ===\n\n");

  benchmarkMatMul(128, 128, 128);
  benchmarkMatMul(512, 512, 512);
  benchmarkMatMul(1024, 1024, 1024);

  return 0;
}
