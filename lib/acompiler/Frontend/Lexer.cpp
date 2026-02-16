#include "acompiler/Frontend/Lexer.h"

#include <sstream>

namespace acompiler {

std::vector<std::string> Lexer::tokenize(const std::string& line) const {
  std::vector<std::string> tokens;
  std::stringstream ss(line);
  std::string token;
  while (ss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

}  // namespace acompiler
