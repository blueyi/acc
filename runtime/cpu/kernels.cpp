#include "acompiler_runtime.h"

void acompiler_runtime_relu_f32(const float* input, float* output, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    output[i] = input[i] > 0.0F ? input[i] : 0.0F;
  }
}
