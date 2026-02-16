#include <iostream>
#include <vector>

#include "acompiler/Runtime/Executor.h"
#include "acompiler/Runtime/Tensor.h"

int main() {
  acompiler::Tensor lhs({4}, {1.0F, -2.0F, 3.0F, -4.0F});
  acompiler::Tensor rhs({4}, {0.5F, 2.5F, -1.0F, 1.0F});
  acompiler::Executor executor;

  auto out = executor.runElementwiseAddRelu(lhs, rhs);
  std::cout << "acompiler-run output:";
  for (float v : out.data()) {
    std::cout << " " << v;
  }
  std::cout << "\n";
  return 0;
}
