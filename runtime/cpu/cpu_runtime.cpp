//===- cpu_runtime.cpp - CPU Runtime Implementation ------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "cpu_runtime.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {

void *ac_alloc(size_t size, size_t alignment) {
  void *ptr = nullptr;
  if (posix_memalign(&ptr, alignment, size) != 0)
    return nullptr;
  memset(ptr, 0, size);
  return ptr;
}

void ac_free(void *ptr) { free(ptr); }

void ac_print_tensor_f32(float *data, int64_t *shape, int64_t rank) {
  printf("Tensor<");
  int64_t total = 1;
  for (int64_t i = 0; i < rank; ++i) {
    if (i > 0)
      printf("x");
    printf("%ld", shape[i]);
    total *= shape[i];
  }
  printf("xf32> = [");
  int64_t printLimit = total < 10 ? total : 10;
  for (int64_t i = 0; i < printLimit; ++i) {
    if (i > 0)
      printf(", ");
    printf("%.4f", data[i]);
  }
  if (total > 10)
    printf(", ...");
  printf("]\n");
}

double ac_get_time_ms() {
  auto now = std::chrono::steady_clock::now();
  auto duration = now.time_since_epoch();
  return std::chrono::duration<double, std::milli>(duration).count();
}

void ac_cpu_init() {
  // TODO: Initialize thread pool, set CPU affinity, etc.
}

void ac_cpu_finalize() {
  // TODO: Clean up resources
}

} // extern "C"
