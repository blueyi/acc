//===- Lexer.h - ACompiler DSL Lexer ----------------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Lexical analyzer for the ACompiler DSL. Converts source text into a stream
// of tokens that the parser can consume.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_FRONTEND_LEXER_H
#define ACOMPILER_FRONTEND_LEXER_H

#include "ACompiler/Frontend/Token.h"
#include <string>
#include <vector>

namespace acompiler {

class Lexer {
public:
  explicit Lexer(std::string source);

  /// Tokenize the entire source and return the token stream.
  std::vector<Token> tokenize();

  /// Get the next token from the source.
  Token nextToken();

  /// Peek at the next token without consuming it.
  Token peekToken();

private:
  std::string source_;
  size_t pos_ = 0;
  size_t line_ = 1;
  size_t col_ = 1;

  char currentChar() const;
  char advance();
  void skipWhitespace();
  void skipComment();
  Token readIdentifier();
  Token readNumber();
  Token readString();
};

} // namespace acompiler

#endif // ACOMPILER_FRONTEND_LEXER_H
