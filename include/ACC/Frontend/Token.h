//===- Token.h - ACC Token Definition ----------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief Token and token kinds for the ACC DSL lexer. */

#ifndef ACC_FRONTEND_TOKEN_H
#define ACC_FRONTEND_TOKEN_H

#include <string>

namespace acc {

/// Token kind produced by the lexer.
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

/// A single token: kind, text value, and source location.
struct Token {
  TokenKind kind;     ///< Token kind.
  std::string value;  ///< Lexeme text.
  size_t line;        ///< 1-based line number.
  size_t col;         ///< 1-based column number.

  Token() : kind(TokenKind::Eof), line(0), col(0) {}
  /// \param k Token kind.
  /// \param v Lexeme value.
  /// \param l Line number.
  /// \param c Column number.
  Token(TokenKind k, std::string v, size_t l, size_t c)
      : kind(k), value(std::move(v)), line(l), col(c) {}
};

/// Convert a TokenKind to its string representation (for debugging).
/// \param kind The token kind.
/// \return Human-readable string for \p kind.
const char *tokenKindToString(TokenKind kind);

} // namespace acc

#endif // ACC_FRONTEND_TOKEN_H
