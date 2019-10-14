// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include "Lexer.h"
#include <cctype>
#include <cstdint>
#include <exception>

void Lexer::advance() {
  pos += 1;
  if (pos > text.size() - 1)
    cur_char = '\0';
  else
    cur_char = text[pos];
}
void Lexer::skip_whitespace() {
  while (cur_char != '\0' && std::isspace(cur_char)) {
    advance();
  }
}
int Lexer::integer() {
  std::string result{};
  while (cur_char != '\0' && std::isdigit(cur_char)) {
    result.push_back(cur_char);
    advance();
  }
  return std::atoi(result.c_str());
}
std::unique_ptr<BaseToken> Lexer::get_next_token() {
  while (cur_char != '\0') {
    if (std::isspace(cur_char))
      skip_whitespace();

    if (std::isdigit(cur_char)) {
      auto num = integer();
      return std::make_unique<Token>(INTEGER, num);
    }
    if (cur_char == '+') {
      advance();
      return std::make_unique<BaseToken>(PLUS);
    }
    if (cur_char == '-') {
      advance();
      return std::make_unique<BaseToken>(MINUS);
    }
    if (cur_char == '*') {
      advance();
      return std::make_unique<BaseToken>(MUL);
    }
    if (cur_char == '/') {
      advance();
      return std::make_unique<BaseToken>(DIV);
    }
    if (cur_char == '(') {
      advance();
      return std::make_unique<BaseToken>(LPARENT);
    }
    if (cur_char == ')') {
      advance();
      return std::make_unique<BaseToken>(RPARENT);
    }

    throw std::logic_error("Invalid character");
  }
  return std::make_unique<BaseToken>(EOF_T);
}
