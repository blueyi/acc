#include "acompiler/Frontend/AST.h"

#include <utility>

namespace acompiler {

void ASTModule::addNode(ASTNode node) { nodes_.push_back(std::move(node)); }

const std::vector<ASTNode>& ASTModule::nodes() const { return nodes_; }

}  // namespace acompiler
