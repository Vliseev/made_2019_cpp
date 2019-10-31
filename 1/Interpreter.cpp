// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Interpreter.h"
#include "Lexer.h"

void Interpreter::Eat(TokenType t) {
    if (t == cur_tok_->type_)
        cur_tok_ = std::move(lex_.GetNextToken());
    else
        throw std::logic_error("Invalid syntax");
}

int Interpreter::Expr() {
    auto res = Term();

    while (cur_tok_->type_ == PLUS || cur_tok_->type_ == MINUS) {
        if (cur_tok_->type_ == PLUS) {
            Eat(PLUS);
            res += Term();
        } else if (cur_tok_->type_ == MINUS) {
            Eat(MINUS);
            res -= Term();
        }
    }
    return res;
}

int Interpreter::Term() {
    auto res = Factor();
    while (cur_tok_->type_ == MUL || cur_tok_->type_ == DIV) {
        if (cur_tok_->type_ == MUL) {
            Eat(MUL);
            res *= Factor();
        } else if (cur_tok_->type_ == DIV) {
            Eat(DIV);
            res /= Factor();
        }
    }
    return res;
}

int Interpreter::Factor() {
    if (cur_tok_->type_ == INTEGER) {
        auto val = cur_tok_->GetVal();
        Eat(INTEGER);
        return val;
    } else if (cur_tok_->type_ == LPARENT) {
        Eat(LPARENT);
        auto val = Expr();
        Eat(RPARENT);
        return val;
    } else if (cur_tok_->type_ == MINUS) {
        Eat(MINUS);
        return -Expr();
    } else {
        throw std::logic_error("invalid syntax");
    }
}

Interpreter::Interpreter(Lexer &lex_t) : lex_(lex_t) {
    cur_tok_ = lex_.GetNextToken();
}
