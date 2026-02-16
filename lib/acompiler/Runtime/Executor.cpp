#include "acompiler/Runtime/Executor.h"

#include <algorithm>
#include <stdexcept>

namespace acompiler {

Tensor Executor::runElementwiseAddRelu(const Tensor& lhs, const Tensor& rhs) const {
  if (lhs.data().size() != rhs.data().size()) {
    throw std::runtime_error("shape mismatch for elementwise add");
  }

  Tensor out(lhs.shape(), lhs.data());
  auto& outData = out.mutableData();
  for (size_t i = 0; i < outData.size(); ++i) {
    outData[i] = std::max(0.0F, lhs.data()[i] + rhs.data()[i]);
  }
  return out;
}

}  // namespace acompiler
