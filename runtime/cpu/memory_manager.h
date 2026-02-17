//===- memory_manager.h - CPU Memory Management ----------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef AC_RUNTIME_CPU_MEMORY_MANAGER_H
#define AC_RUNTIME_CPU_MEMORY_MANAGER_H

#include <cstddef>
#include <cstdint>

namespace acc {
namespace runtime {

/// Simple memory pool for tensor allocations on CPU.
/// Supports aligned allocation and buffer reuse.
class CPUMemoryManager {
public:
  static CPUMemoryManager &instance();

  void *allocate(size_t size, size_t alignment = 64);
  void deallocate(void *ptr);

  /// Report memory usage statistics.
  void reportStats() const;

private:
  CPUMemoryManager() = default;
  size_t totalAllocated_ = 0;
  size_t peakAllocated_ = 0;
  size_t currentAllocated_ = 0;
};

} // namespace runtime
} // namespace acc

#endif // AC_RUNTIME_CPU_MEMORY_MANAGER_H
