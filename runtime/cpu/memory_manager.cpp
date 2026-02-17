//===- memory_manager.cpp - CPU Memory Management Implementation -*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "memory_manager.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace acc {
namespace runtime {

CPUMemoryManager &CPUMemoryManager::instance() {
  static CPUMemoryManager mgr;
  return mgr;
}

void *CPUMemoryManager::allocate(size_t size, size_t alignment) {
  void *ptr = nullptr;
  if (posix_memalign(&ptr, alignment, size) != 0)
    return nullptr;
  memset(ptr, 0, size);

  totalAllocated_ += size;
  currentAllocated_ += size;
  if (currentAllocated_ > peakAllocated_)
    peakAllocated_ = currentAllocated_;

  return ptr;
}

void CPUMemoryManager::deallocate(void *ptr) {
  // Note: We don't track individual allocation sizes in this simple version.
  // A production implementation would use a map<ptr, size>.
  free(ptr);
}

void CPUMemoryManager::reportStats() const {
  printf("\n=== CPU Memory Manager Stats ===\n");
  printf("  Total allocated:   %zu bytes\n", totalAllocated_);
  printf("  Peak allocated:    %zu bytes\n", peakAllocated_);
  printf("  Current allocated: %zu bytes\n", currentAllocated_);
  printf("================================\n");
}

} // namespace runtime
} // namespace acc
