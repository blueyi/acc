//===- Lexer.cpp - ACompiler DSL Lexer Implementation -----------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Frontend/Lexer.h"

namespace acompiler {

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;
  while (true) {
    Token tok = nextToken();
    tokens.push_back(tok);
    if (tok.kind == TokenKind::Eof || tok.kind == TokenKind::Error)
      break;
  }
  return tokens;
}

Token Lexer::nextToken() {
  skipWhitespace();

  if (pos_ >= source_.size())
    return Token(TokenKind::Eof, "", line_, col_);

  // TODO: Implement full lexer
  // 1. Identify keywords (model, input, output, layer, tensor, etc.)
  // 2. Identify identifiers and numbers
  // 3. Identify operators and punctuation
  // 4. Handle string literals
  // 5. Handle comments (// and /* */)

  return Token(TokenKind::Eof, "", line_, col_);
}

Token Lexer::peekToken() {
  size_t savedPos = pos_;
  size_t savedLine = line_;
  size_t savedCol = col_;
  Token tok = nextToken();
  pos_ = savedPos;
  line_ = savedLine;
  col_ = savedCol;
  return tok;
}

char Lexer::currentChar() const {
  return pos_ < source_.size() ? source_[pos_] : '\0';
}

char Lexer::advance() {
  char c = currentChar();
  if (c == '\n') {
    line_++;
    col_ = 1;
  } else {
    col_++;
  }
  pos_++;
  return c;
}

void Lexer::skipWhitespace() {
  while (pos_ < source_.size() && std::isspace(source_[pos_]))
    advance();
}

void Lexer::skipComment() {
  // TODO: Implement comment skipping
}

Token Lexer::readIdentifier() {
  // TODO: Implement identifier reading
  return Token(TokenKind::Error, "", line_, col_);
}

Token Lexer::readNumber() {
  // TODO: Implement number reading
  return Token(TokenKind::Error, "", line_, col_);
}

Token Lexer::readString() {
  // TODO: Implement string literal reading
  return Token(TokenKind::Error, "", line_, col_);
}

} // namespace acompiler
