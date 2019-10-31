#ifndef PARSER_LEXER_H
#define PARSER_LEXER_H
#include <memory>
#include <string>

enum TokenType { INTEGER, MUL, DIV, PLUS, MINUS, EOF_T, LPARENT, RPARENT };

struct BaseToken {
    explicit BaseToken(const TokenType &type) : type_(type) {}
    TokenType type_;
    virtual int GetVal();
};

struct Token : BaseToken {
    Token(const TokenType &type, const int &val) : BaseToken(type), val_(val) {}
    int val_;
    int GetVal() override;
};

class Lexer {
   private:
    std::string text_{};
    size_t pos_{};
    char cur_char_{};

   public:
    void Advance();
    void SkipWhitespace();
    int Integer();
    explicit Lexer(const std::string &text_in) : text_(text_in) {
        cur_char_ = text_in[0];
    };

    std::unique_ptr<BaseToken> GetNextToken();
};

#endif    // PARSER_LEXER_H
