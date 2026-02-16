#include <chrono>
#include <iostream>
#include <vector>

int main() {
  constexpr int n = 64;
  std::vector<float> a(n * n, 1.0F);
  std::vector<float> b(n * n, 2.0F);
  std::vector<float> c(n * n, 0.0F);

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      float sum = 0.0F;
      for (int k = 0; k < n; ++k) {
        sum += a[i * n + k] * b[k * n + j];
      }
      c[i * n + j] = sum;
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "matmul benchmark done in " << ms << " ms\n";
  return 0;
}
