#include "acompiler/Frontend/Parser.h"

#include <stdexcept>

#include "acompiler/Frontend/Lexer.h"

namespace acompiler {

ASTNode Parser::parseLine(const std::string& line) const {
  Lexer lexer;
  auto tokens = lexer.tokenize(line);
  if (tokens.empty()) {
    throw std::runtime_error("empty statement");
  }

  ASTNode node;
  node.op = tokens.front();
  node.operands.assign(tokens.begin() + 1, tokens.end());
  return node;
}

ASTModule Parser::parseLines(const std::vector<std::string>& lines) const {
  ASTModule module;
  for (const auto& line : lines) {
    if (line.empty()) {
      continue;
    }
    module.addNode(parseLine(line));
  }
  return module;
}

}  // namespace acompiler
