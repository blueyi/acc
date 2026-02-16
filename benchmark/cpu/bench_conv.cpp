#include <chrono>
#include <iostream>
#include <vector>

int main() {
  constexpr int h = 32;
  constexpr int w = 32;
  std::vector<float> input(h * w, 1.0F);
  std::vector<float> kernel(9, 0.1111F);
  std::vector<float> out((h - 2) * (w - 2), 0.0F);

  auto start = std::chrono::steady_clock::now();
  for (int i = 1; i < h - 1; ++i) {
    for (int j = 1; j < w - 1; ++j) {
      float sum = 0.0F;
      int idx = 0;
      for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
          sum += input[(i + di) * w + (j + dj)] * kernel[idx++];
        }
      }
      out[(i - 1) * (w - 2) + (j - 1)] = sum;
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "conv benchmark done in " << ms << " ms\n";
  return 0;
}
