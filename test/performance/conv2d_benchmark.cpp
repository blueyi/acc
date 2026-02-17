// Conv2D 性能基准测试
// test/performance/conv2d_benchmark.cpp

#include "benchmark_framework.cpp"
#include <random>
#include <memory>

using namespace acompiler::benchmark;

// 简单的Conv2D实现（用于测试）
void conv2d_naive(const float *input, const float *weight, float *output,
                  int N, int C, int H, int W, int K, int R, int S,
                  int stride, int padding) {
  int H_out = (H + 2*padding - R) / stride + 1;
  int W_out = (W + 2*padding - S) / stride + 1;
  
  for (int n = 0; n < N; ++n) {
    for (int k = 0; k < K; ++k) {
      for (int h_out = 0; h_out < H_out; ++h_out) {
        for (int w_out = 0; w_out < W_out; ++w_out) {
          float sum = 0.0f;
          
          for (int c = 0; c < C; ++c) {
            for (int r = 0; r < R; ++r) {
              for (int s = 0; s < S; ++s) {
                int h_in = h_out * stride - padding + r;
                int w_in = w_out * stride - padding + s;
                
                if (h_in >= 0 && h_in < H && w_in >= 0 && w_in < W) {
                  int input_idx = ((n*C + c)*H + h_in)*W + w_in;
                  int weight_idx = ((k*C + c)*R + r)*S + s;
                  sum += input[input_idx] * weight[weight_idx];
                }
              }
            }
          }
          
          int output_idx = ((n*K + k)*H_out + h_out)*W_out + w_out;
          output[output_idx] = sum;
        }
      }
    }
  }
}

class Conv2DBenchmark {
public:
  Conv2DBenchmark(int N, int C, int H, int W, int K, int R, int S,
                  int stride = 1, int padding = 0)
      : N(N), C(C), H(H), W(W), K(K), R(R), S(S),
        stride(stride), padding(padding) {
    
    H_out = (H + 2*padding - R) / stride + 1;
    W_out = (W + 2*padding - S) / stride + 1;
    
    // 分配内存
    input.resize(N * C * H * W);
    weight.resize(K * C * R * S);
    output.resize(N * K * H_out * W_out);
    
    // 随机初始化
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    for (auto &val : input) val = dis(gen);
    for (auto &val : weight) val = dis(gen);
  }
  
  void runNaive() {
    conv2d_naive(input.data(), weight.data(), output.data(),
                N, C, H, W, K, R, S, stride, padding);
  }
  
  void runOptimized() {
    // TODO: 调用ACompiler生成的优化实现
    // acompiler_conv2d(input.data(), weight.data(), output.data(), ...);
    
    // 暂时使用naive实现
    runNaive();
  }
  
  int64_t getFlops() const {
    return computeConv2DFlops(N, C, H, W, K, R, S, H_out, W_out);
  }
  
  int64_t getMemoryBytes() const {
    int64_t input_size = N * C * H * W;
    int64_t weight_size = K * C * R * S;
    int64_t output_size = N * K * H_out * W_out;
    return (input_size + weight_size + output_size) * sizeof(float);
  }

private:
  int N, C, H, W, K, R, S, stride, padding;
  int H_out, W_out;
  std::vector<float> input, weight, output;
};

int main() {
  BenchmarkFramework framework(10, 100);
  
  // 测试配置
  struct Config {
    std::string name;
    int N, C, H, W, K, R, S, stride, padding;
  };
  
  std::vector<Config> configs = {
    {"ResNet-18 conv1", 1, 3, 224, 224, 64, 7, 7, 2, 3},
    {"ResNet-18 layer1", 1, 64, 56, 56, 64, 3, 3, 1, 1},
    {"ResNet-18 layer2", 1, 128, 28, 28, 128, 3, 3, 1, 1},
    {"ResNet-50 bottleneck", 1, 256, 56, 56, 64, 1, 1, 1, 0},
    {"MobileNetV2 depthwise", 1, 32, 112, 112, 32, 3, 3, 1, 1},
  };
  
  std::cout << "=================================================\n";
  std::cout << "         Conv2D Performance Benchmark           \n";
  std::cout << "=================================================\n\n";
  
  for (const auto &config : configs) {
    Conv2DBenchmark bench(config.N, config.C, config.H, config.W,
                         config.K, config.R, config.S,
                         config.stride, config.padding);
    
    std::cout << "\n--- Configuration: " << config.name << " ---\n";
    std::cout << "  Input:  [" << config.N << ", " << config.C << ", "
              << config.H << ", " << config.W << "]\n";
    std::cout << "  Weight: [" << config.K << ", " << config.C << ", "
              << config.R << ", " << config.S << "]\n";
    std::cout << "  FLOPS:  " << bench.getFlops() / 1e9 << " GFLOP\n";
    std::cout << "  Memory: " << bench.getMemoryBytes() / 1e6 << " MB\n";
    
    // 运行基准测试
    auto naive_stats = framework.run(
        config.name + " (naive)",
        [&]() { bench.runNaive(); },
        bench.getFlops(),
        bench.getMemoryBytes());
    
    framework.printResults(config.name + " (naive)", naive_stats);
    
    auto opt_stats = framework.run(
        config.name + " (optimized)",
        [&]() { bench.runOptimized(); },
        bench.getFlops(),
        bench.getMemoryBytes());
    
    framework.printResults(config.name + " (optimized)", opt_stats);
    
    // 比较结果
    framework.compareResults(config.name, opt_stats, naive_stats);
  }
  
  std::cout << "\n=================================================\n";
  std::cout << "  Performance targets for ACompiler:\n";
  std::cout << "  - Conv2D (typical): >60% of optimized BLAS\n";
  std::cout << "  - MatMul (large):   >65% of MKL/OpenBLAS\n";
  std::cout << "  - ResNet-18 E2E:    >65% of ONNX Runtime\n";
  std::cout << "=================================================\n";
  
  return 0;
}
