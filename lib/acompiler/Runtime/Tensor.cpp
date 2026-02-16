#include "acompiler/Runtime/Tensor.h"

#include <utility>

namespace acompiler {

Tensor::Tensor(std::vector<int64_t> shape, std::vector<float> data)
    : shape_(std::move(shape)), data_(std::move(data)) {}

Tensor Tensor::fromScalar(float value) { return Tensor({1}, {value}); }

const std::vector<int64_t>& Tensor::shape() const { return shape_; }

const std::vector<float>& Tensor::data() const { return data_; }

std::vector<float>& Tensor::mutableData() { return data_; }

}  // namespace acompiler
