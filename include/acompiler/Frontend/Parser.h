#pragma once

#include <string>
#include <vector>

#include "acompiler/Frontend/AST.h"

namespace acompiler {

class Parser {
 public:
  ASTNode parseLine(const std::string& line) const;
  ASTModule parseLines(const std::vector<std::string>& lines) const;
};

}  // namespace acompiler
