//===- Parser.cpp - ACC DSL Parser Implementation ---------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Frontend/Parser.h"

namespace acc {

Parser::Parser(Lexer &lexer) : lexer_(lexer) { advance(); }

Token Parser::advance() {
  currentToken_ = lexer_.nextToken();
  return currentToken_;
}

bool Parser::expect(TokenKind kind) {
  if (currentToken_.kind != kind)
    return false;
  advance();
  return true;
}

std::unique_ptr<ModuleAST> Parser::parseModule() {
  auto module = std::make_unique<ModuleAST>();

  // TODO: Implement module parsing
  // Parse model declarations and compile declarations

  return module;
}

std::unique_ptr<ModelDeclAST> Parser::parseModelDecl() {
  // TODO: Implement model declaration parsing
  return nullptr;
}

std::unique_ptr<LayerDeclAST> Parser::parseLayerDecl() {
  // TODO: Implement layer declaration parsing
  return nullptr;
}

std::unique_ptr<ExprAST> Parser::parseExpression() {
  // TODO: Implement expression parsing
  return nullptr;
}

std::unique_ptr<CompileDeclAST> Parser::parseCompileDecl() {
  // TODO: Implement compile declaration parsing
  return nullptr;
}

} // namespace acc
