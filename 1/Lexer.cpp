// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Lexer.h"
#include <cctype>

void Lexer::Advance() {
    pos_ += 1;
    if (pos_ > text_.size() - 1)
        cur_char_ = '\0';
    else
        cur_char_ = text_[pos_];
}
void Lexer::SkipWhitespace() {
    while (cur_char_ != '\0' && std::isspace(cur_char_)) {
        Advance();
    }
}
int Lexer::Integer() {
    std::string result{};
    while (cur_char_ != '\0' && std::isdigit(cur_char_)) {
        result.push_back(cur_char_);
        Advance();
    }
    return std::atoi(result.c_str());
}
std::unique_ptr<BaseToken> Lexer::GetNextToken() {
    while (cur_char_ != '\0') {
        if (std::isspace(cur_char_))
            SkipWhitespace();

        if (std::isdigit(cur_char_)) {
            auto num = Integer();
            return std::make_unique<Token>(INTEGER, num);
        }
        if (cur_char_ == '+') {
            Advance();
            return std::make_unique<BaseToken>(PLUS);
        }
        if (cur_char_ == '-') {
            Advance();
            return std::make_unique<BaseToken>(MINUS);
        }
        if (cur_char_ == '*') {
            Advance();
            return std::make_unique<BaseToken>(MUL);
        }
        if (cur_char_ == '/') {
            Advance();
            return std::make_unique<BaseToken>(DIV);
        }
        if (cur_char_ == '(') {
            Advance();
            return std::make_unique<BaseToken>(LPARENT);
        }
        if (cur_char_ == ')') {
            Advance();
            return std::make_unique<BaseToken>(RPARENT);
        }

        throw std::logic_error("Invalid character");
    }
    return std::make_unique<BaseToken>(EOF_T);
}
int BaseToken::GetVal() {
    throw std::logic_error("Base token has no value");
}
int Token::GetVal() {
    return val_;
}
