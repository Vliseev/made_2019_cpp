

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H
#include <memory>

class Lexer;

class Interpreter {
private:
  Lexer *lex{};
  std::unique_ptr<class BaseToken> cur_tok{};

public:
  Interpreter(Lexer *lex_t);
  //void eat(TOKEN_TYPE t);
  void eat(int t);
  int factor();
  int expr();
  int term();
};

#endif // PARSER_PARSER_H
