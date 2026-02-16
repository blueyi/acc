#pragma once

#include <string>
#include <vector>

namespace acompiler {

struct ASTNode {
  std::string op;
  std::vector<std::string> operands;
};

class ASTModule {
 public:
  void addNode(ASTNode node);
  const std::vector<ASTNode>& nodes() const;

 private:
  std::vector<ASTNode> nodes_;
};

}  // namespace acompiler
