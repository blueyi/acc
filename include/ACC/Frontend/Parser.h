//===- Parser.h - ACC DSL Parser --------------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Recursive descent parser for the ACC DSL. Converts a token stream
// into an Abstract Syntax Tree (AST).
//
//===----------------------------------------------------------------------===//

#ifndef ACC_FRONTEND_PARSER_H
#define ACC_FRONTEND_PARSER_H

#include "ACC/Frontend/AST.h"
#include "ACC/Frontend/Lexer.h"
#include <memory>

namespace acc {

class Parser {
public:
  explicit Parser(Lexer &lexer);

  /// Parse the complete source into a module AST node.
  std::unique_ptr<ModuleAST> parseModule();

private:
  Lexer &lexer_;
  Token currentToken_;

  Token advance();
  bool expect(TokenKind kind);

  std::unique_ptr<ModelDeclAST> parseModelDecl();
  std::unique_ptr<LayerDeclAST> parseLayerDecl();
  std::unique_ptr<ExprAST> parseExpression();
  std::unique_ptr<CompileDeclAST> parseCompileDecl();
};

} // namespace acc

#endif // ACC_FRONTEND_PARSER_H
