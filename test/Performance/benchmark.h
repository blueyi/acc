//===- benchmark.h - ACC Performance Benchmark Framework --*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Lightweight benchmark framework for measuring operator and model
// performance. Supports warm-up, statistical analysis, and comparison
// with reference implementations (e.g., ONNX Runtime).
//
//===----------------------------------------------------------------------===//

#ifndef AC_TEST_PERFORMANCE_BENCHMARK_H
#define AC_TEST_PERFORMANCE_BENCHMARK_H

#include <chrono>
#include <cmath>
#include <cstdio>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

namespace acc {
namespace benchmark {

struct BenchmarkResult {
  std::string name;
  double avgTimeMs;
  double medianTimeMs;
  double minTimeMs;
  double maxTimeMs;
  double stdDevMs;
  double gflops;
  double bandwidthGBs;
  int iterations;
};

class OpBenchmark {
public:
  /// Run a benchmark with warm-up and statistical analysis.
  static BenchmarkResult run(const std::string &name,
                              std::function<void()> fn,
                              int warmup = 10,
                              int iterations = 100,
                              double flops = 0.0,
                              double bytes = 0.0) {
    // Warm-up
    for (int i = 0; i < warmup; ++i) {
      fn();
    }

    // Measurement
    std::vector<double> times;
    times.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      fn();
      auto end = std::chrono::high_resolution_clock::now();
      double ms =
          std::chrono::duration<double, std::milli>(end - start).count();
      times.push_back(ms);
    }

    // Compute statistics
    std::sort(times.begin(), times.end());
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double avg = sum / iterations;
    double median = times[iterations / 2];
    double minT = times.front();
    double maxT = times.back();

    double variance = 0.0;
    for (double t : times) {
      variance += (t - avg) * (t - avg);
    }
    double stddev = std::sqrt(variance / iterations);

    double gflops = (flops > 0) ? (flops / (avg * 1e6)) : 0.0;
    double bandwidth = (bytes > 0) ? (bytes / (avg * 1e6)) : 0.0;

    return BenchmarkResult{name,  avg,        median, minT, maxT,
                            stddev, gflops, bandwidth, iterations};
  }

  /// Print a benchmark result.
  static void printResult(const BenchmarkResult &r) {
    printf("%-30s  avg: %8.3f ms  median: %8.3f ms  "
           "min: %8.3f ms  std: %6.3f ms",
           r.name.c_str(), r.avgTimeMs, r.medianTimeMs,
           r.minTimeMs, r.stdDevMs);
    if (r.gflops > 0)
      printf("  %.2f GFLOPS", r.gflops);
    if (r.bandwidthGBs > 0)
      printf("  %.2f GB/s", r.bandwidthGBs);
    printf("\n");
  }

  /// Print comparison between ACC and a reference.
  static void printComparison(const BenchmarkResult &ac,
                               const BenchmarkResult &ref) {
    printf("\n=== %s Performance Comparison ===\n", ac.name.c_str());
    printf("  ACC:  %8.3f ms", ac.avgTimeMs);
    if (ac.gflops > 0) printf("  (%.2f GFLOPS)", ac.gflops);
    printf("\n");
    printf("  Reference:  %8.3f ms", ref.avgTimeMs);
    if (ref.gflops > 0) printf("  (%.2f GFLOPS)", ref.gflops);
    printf("\n");
    printf("  Speedup:    %.2fx\n", ref.avgTimeMs / ac.avgTimeMs);
    printf("  Relative:   %.1f%%\n",
           (ref.avgTimeMs / ac.avgTimeMs) * 100.0);
    printf("==========================================\n");
  }
};

/// Compute FLOPS for common operations.
inline double matmulFlops(int64_t M, int64_t N, int64_t K) {
  return 2.0 * M * N * K;
}

inline double conv2dFlops(int64_t N, int64_t C, int64_t K,
                           int64_t H_out, int64_t W_out,
                           int64_t R, int64_t S) {
  return 2.0 * N * K * H_out * W_out * C * R * S;
}

} // namespace benchmark
} // namespace acc

#endif // AC_TEST_PERFORMANCE_BENCHMARK_H
