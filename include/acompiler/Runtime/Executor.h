#pragma once

#include "acompiler/Runtime/Tensor.h"

namespace acompiler {

class Executor {
 public:
  Tensor runElementwiseAddRelu(const Tensor& lhs, const Tensor& rhs) const;
};

}  // namespace acompiler
