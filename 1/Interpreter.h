

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H
#include <memory>
#include "Lexer.h"

class Lexer;

class Interpreter {
   private:
    Lexer &lex_;
    std::unique_ptr<class BaseToken> cur_tok_{};

   public:
    explicit Interpreter(Lexer &lex_t);
    void Eat(TokenType t);
    int Factor();
    int Expr();
    int Term();
};

#endif    // PARSER_PARSER_H
