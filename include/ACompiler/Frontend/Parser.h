//===- Parser.h - ACompiler DSL Parser --------------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Recursive descent parser for the ACompiler DSL. Converts a token stream
// into an Abstract Syntax Tree (AST).
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_FRONTEND_PARSER_H
#define ACOMPILER_FRONTEND_PARSER_H

#include "ACompiler/Frontend/AST.h"
#include "ACompiler/Frontend/Lexer.h"
#include <memory>

namespace acompiler {

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

} // namespace acompiler

#endif // ACOMPILER_FRONTEND_PARSER_H
