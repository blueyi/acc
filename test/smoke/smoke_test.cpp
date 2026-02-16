#include <iostream>
#include <vector>

#include "acompiler/Runtime/Executor.h"
#include "acompiler/Runtime/Tensor.h"

int main() {
  acompiler::Tensor lhs({3}, {1.0F, -1.0F, 2.0F});
  acompiler::Tensor rhs({3}, {2.0F, 2.0F, -5.0F});
  acompiler::Executor executor;

  auto out = executor.runElementwiseAddRelu(lhs, rhs);
  const std::vector<float> expected{3.0F, 1.0F, 0.0F};
  if (out.data() != expected) {
    std::cerr << "smoke test failed\n";
    return 1;
  }

  std::cout << "smoke test passed\n";
  return 0;
}
