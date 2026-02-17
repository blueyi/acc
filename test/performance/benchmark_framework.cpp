// 性能基准测试框架
// test/performance/benchmark_framework.cpp

#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>

namespace acompiler {
namespace benchmark {

/// 性能统计结果
struct BenchmarkStats {
  double mean;        // 平均值 (ms)
  double stddev;      // 标准差 (ms)
  double min;         // 最小值 (ms)
  double max;         // 最大值 (ms)
  double median;      // 中位数 (ms)
  double throughput;  // 吞吐量 (GFLOPS)
  double bandwidth;   // 内存带宽 (GB/s)
};

/// 基准测试框架类
class BenchmarkFramework {
public:
  BenchmarkFramework(int warmup = 10, int iterations = 100)
      : warmupIterations(warmup), benchmarkIterations(iterations) {}

  /// 运行基准测试
  template<typename Func>
  BenchmarkStats run(const std::string &name, Func &&func, 
                     int64_t flops = 0, int64_t bytes = 0) {
    std::cout << "Running benchmark: " << name << "\n";
    std::cout << "  Warmup: " << warmupIterations << " iterations\n";
    std::cout << "  Benchmark: " << benchmarkIterations << " iterations\n";
    
    // 预热
    for (int i = 0; i < warmupIterations; ++i) {
      func();
    }
    
    // 正式测试
    std::vector<double> timings;
    timings.reserve(benchmarkIterations);
    
    for (int i = 0; i < benchmarkIterations; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      func();
      auto end = std::chrono::high_resolution_clock::now();
      
      double elapsed = std::chrono::duration<double, std::milli>(
          end - start).count();
      timings.push_back(elapsed);
    }
    
    // 计算统计信息
    return computeStats(timings, flops, bytes);
  }
  
  /// 打印结果
  void printResults(const std::string &name, const BenchmarkStats &stats) {
    std::cout << "\n=== " << name << " Results ===\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "  Mean:       " << stats.mean << " ms\n";
    std::cout << "  Std Dev:    " << stats.stddev << " ms\n";
    std::cout << "  Min:        " << stats.min << " ms\n";
    std::cout << "  Max:        " << stats.max << " ms\n";
    std::cout << "  Median:     " << stats.median << " ms\n";
    
    if (stats.throughput > 0) {
      std::cout << "  Throughput: " << stats.throughput << " GFLOPS\n";
    }
    
    if (stats.bandwidth > 0) {
      std::cout << "  Bandwidth:  " << stats.bandwidth << " GB/s\n";
    }
    std::cout << std::endl;
  }
  
  /// 比较两个实现
  void compareResults(const std::string &name,
                     const BenchmarkStats &acompiler,
                     const BenchmarkStats &reference) {
    std::cout << "\n=== " << name << " Comparison ===\n";
    std::cout << std::fixed << std::setprecision(3);
    
    std::cout << "  ACompiler:  " << acompiler.mean << " ms";
    if (acompiler.throughput > 0) {
      std::cout << ", " << acompiler.throughput << " GFLOPS";
    }
    std::cout << "\n";
    
    std::cout << "  Reference:  " << reference.mean << " ms";
    if (reference.throughput > 0) {
      std::cout << ", " << reference.throughput << " GFLOPS";
    }
    std::cout << "\n";
    
    double speedup = reference.mean / acompiler.mean;
    std::cout << "  Speedup:    " << speedup << "x";
    
    double percentage = (acompiler.mean / reference.mean) * 100.0;
    std::cout << " (" << percentage << "% of reference)\n";
    
    if (speedup >= 1.0) {
      std::cout << "  Status:     ✓ FASTER\n";
    } else if (percentage >= 60.0) {
      std::cout << "  Status:     ✓ ACCEPTABLE (>60%)\n";
    } else {
      std::cout << "  Status:     ✗ NEEDS IMPROVEMENT\n";
    }
    
    std::cout << std::endl;
  }

private:
  BenchmarkStats computeStats(std::vector<double> &timings,
                              int64_t flops, int64_t bytes) {
    BenchmarkStats stats{};
    
    // 排序用于计算中位数
    std::sort(timings.begin(), timings.end());
    
    // 计算统计量
    stats.min = timings.front();
    stats.max = timings.back();
    stats.median = timings[timings.size() / 2];
    
    double sum = std::accumulate(timings.begin(), timings.end(), 0.0);
    stats.mean = sum / timings.size();
    
    double sq_sum = std::inner_product(
        timings.begin(), timings.end(), timings.begin(), 0.0);
    stats.stddev = std::sqrt(sq_sum / timings.size() - 
                            stats.mean * stats.mean);
    
    // 计算吞吐量（如果提供了FLOPS）
    if (flops > 0) {
      // GFLOPS = (FLOPS / time_in_ms) * 1e-6
      stats.throughput = (flops / stats.mean) * 1e-6;
    }
    
    // 计算带宽（如果提供了字节数）
    if (bytes > 0) {
      // GB/s = (bytes / time_in_ms) * 1e-6
      stats.bandwidth = (bytes / stats.mean) * 1e-6;
    }
    
    return stats;
  }
  
  int warmupIterations;
  int benchmarkIterations;
};

/// 计算卷积的FLOPS
inline int64_t computeConv2DFlops(int64_t N, int64_t C, int64_t H, int64_t W,
                                   int64_t K, int64_t R, int64_t S,
                                   int64_t H_out, int64_t W_out) {
  // 每个输出元素需要 C*R*S 次乘法和加法
  return N * K * H_out * W_out * C * R * S * 2;
}

/// 计算矩阵乘法的FLOPS
inline int64_t computeMatMulFlops(int64_t M, int64_t N, int64_t K) {
  return M * N * K * 2; // 每个元素需要K次乘法和加法
}

/// 计算内存访问字节数
inline int64_t computeMemoryBytes(const std::vector<int64_t> &shapes) {
  int64_t total = 0;
  for (int64_t shape : shapes) {
    total += shape * sizeof(float);
  }
  return total;
}

} // namespace benchmark
} // namespace acompiler
