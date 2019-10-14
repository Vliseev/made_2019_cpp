// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Interpreter.h"
#include "Lexer.h"

/*void Interpreter::eat(TOKEN_TYPE t) {
  if (t == cur_tok->type)
    cur_tok = std::move(lex->get_next_token());
  else
    throw "Invalid syntax";
}*/

void Interpreter::eat(int t) {
  if (t == cur_tok->type)
    cur_tok = std::move(lex->get_next_token());
  else
    throw std::logic_error("Invalid syntax");
}

int Interpreter::expr() {
  auto res = term();

  while (cur_tok->type == PLUS || cur_tok->type == MINUS) {
    if (cur_tok->type == PLUS) {
      eat(PLUS);
      res += term();
    } else if (cur_tok->type == MINUS) {
      eat(MINUS);
      res -= term();
    }
  }
  return res;
}

int Interpreter::term() {
  auto res = factor();
  while (cur_tok->type == MUL || cur_tok->type == DIV) {
    if (cur_tok->type == MUL) {
      eat(MUL);
      res *= factor();
    } else if (cur_tok->type == DIV) {
      eat(DIV);
      res /= factor();
    }
  }
  return res;
}

int Interpreter::factor() {
  if (cur_tok->type == INTEGER) {
    auto val = reinterpret_cast<Token *>(cur_tok.get())->val;
    eat(INTEGER);
    return val;
  } else if (cur_tok->type == LPARENT) {
    eat(LPARENT);
    auto val = expr();
    eat(RPARENT);
    return val;
  } else if (cur_tok->type == MINUS) {
    eat(MINUS);
    return -expr();
  } else {
    throw std::logic_error("invalid syntax");
  }
}

Interpreter::Interpreter(Lexer *lex_t) {
  lex = lex_t;
  cur_tok = lex->get_next_token();
}
