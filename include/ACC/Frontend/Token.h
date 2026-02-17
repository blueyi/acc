//===- Token.h - ACC Token Definition ----------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef ACC_FRONTEND_TOKEN_H
#define ACC_FRONTEND_TOKEN_H

#include <string>

namespace acc {

enum class TokenKind {
  // Literals
  Integer,
  Float,
  String,
  Identifier,

  // Keywords
  KW_model,
  KW_input,
  KW_output,
  KW_layer,
  KW_tensor,
  KW_compile,
  KW_target,
  KW_optimize,
  KW_true,
  KW_false,

  // Operators and punctuation
  LParen,    // (
  RParen,    // )
  LBrace,    // {
  RBrace,    // }
  LBracket,  // [
  RBracket,  // ]
  Comma,     // ,
  Colon,     // :
  Equal,     // =
  Arrow,     // ->
  Dot,       // .

  // Special
  Eof,
  Error
};

struct Token {
  TokenKind kind;
  std::string value;
  size_t line;
  size_t col;

  Token() : kind(TokenKind::Eof), line(0), col(0) {}
  Token(TokenKind k, std::string v, size_t l, size_t c)
      : kind(k), value(std::move(v)), line(l), col(c) {}
};

/// Convert a TokenKind to its string representation (for debugging).
const char *tokenKindToString(TokenKind kind);

} // namespace acc

#endif // ACC_FRONTEND_TOKEN_H
