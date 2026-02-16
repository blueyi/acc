#include "acompiler_runtime.h"

#include <cstdlib>

void* acompiler_runtime_alloc(size_t size_in_bytes) { return std::malloc(size_in_bytes); }

void acompiler_runtime_free(void* ptr) { std::free(ptr); }
