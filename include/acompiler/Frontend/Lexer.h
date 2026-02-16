#pragma once

#include <string>
#include <vector>

namespace acompiler {

class Lexer {
 public:
  std::vector<std::string> tokenize(const std::string& line) const;
};

}  // namespace acompiler
