//===- Lexer.h - ACC DSL Lexer ----------------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Lexical analyzer for the ACC DSL. Converts source text into a stream
// of tokens that the parser can consume.
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief Lexical analyzer for the ACC DSL. */

#ifndef ACC_FRONTEND_LEXER_H
#define ACC_FRONTEND_LEXER_H

#include "ACC/Frontend/Token.h"
#include <string>
#include <vector>

namespace acc {

/// Lexical analyzer: converts source text into a stream of tokens.
class Lexer {
public:
  /// \param source Full source code string.
  explicit Lexer(std::string source);

  /// Tokenize the entire source and return the token stream.
  /// \return Vector of tokens (including Eof at the end).
  std::vector<Token> tokenize();

  /// Get the next token from the source.
  /// \return The next token.
  Token nextToken();

  /// Peek at the next token without consuming it.
  /// \return The next token (unchanged stream position).
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

} // namespace acc

#endif // ACC_FRONTEND_LEXER_H
