//===- Token.cpp - ACC Token Utilities --------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Frontend/Token.h"

namespace acc {

const char *tokenKindToString(TokenKind kind) {
  switch (kind) {
  case TokenKind::Integer:    return "Integer";
  case TokenKind::Float:      return "Float";
  case TokenKind::String:     return "String";
  case TokenKind::Identifier: return "Identifier";
  case TokenKind::KW_model:   return "model";
  case TokenKind::KW_input:   return "input";
  case TokenKind::KW_output:  return "output";
  case TokenKind::KW_layer:   return "layer";
  case TokenKind::KW_tensor:  return "tensor";
  case TokenKind::KW_compile: return "compile";
  case TokenKind::KW_target:  return "target";
  case TokenKind::KW_optimize: return "optimize";
  case TokenKind::KW_true:    return "true";
  case TokenKind::KW_false:   return "false";
  case TokenKind::LParen:     return "(";
  case TokenKind::RParen:     return ")";
  case TokenKind::LBrace:     return "{";
  case TokenKind::RBrace:     return "}";
  case TokenKind::LBracket:   return "[";
  case TokenKind::RBracket:   return "]";
  case TokenKind::Comma:      return ",";
  case TokenKind::Colon:      return ":";
  case TokenKind::Equal:      return "=";
  case TokenKind::Arrow:      return "->";
  case TokenKind::Dot:        return ".";
  case TokenKind::Eof:        return "EOF";
  case TokenKind::Error:      return "ERROR";
  }
  return "UNKNOWN";
}

} // namespace acc
