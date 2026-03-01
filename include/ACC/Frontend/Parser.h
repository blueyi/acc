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

/** \file
 * \brief Recursive descent parser for the ACC DSL. */

#ifndef ACC_FRONTEND_PARSER_H
#define ACC_FRONTEND_PARSER_H

#include "ACC/Frontend/AST.h"
#include "ACC/Frontend/Lexer.h"
#include <memory>

namespace acc {

/// Recursive descent parser: converts a token stream into an AST.
class Parser {
public:
  /// \param lexer Lexer that provides the token stream.
  explicit Parser(Lexer &lexer);

  /// Parse the complete source into a module AST node.
  /// \return The root ModuleAST, or nullptr on parse error.
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
