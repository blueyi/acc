#pragma once

#include <cstdint>
#include <vector>

namespace acompiler {

class Tensor {
 public:
  Tensor() = default;
  Tensor(std::vector<int64_t> shape, std::vector<float> data);

  static Tensor fromScalar(float value);

  const std::vector<int64_t>& shape() const;
  const std::vector<float>& data() const;
  std::vector<float>& mutableData();

 private:
  std::vector<int64_t> shape_;
  std::vector<float> data_;
};

}  // namespace acompiler
