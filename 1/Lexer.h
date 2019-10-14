#ifndef PARSER_LEXER_H
#define PARSER_LEXER_H
#include <memory>
#include <string>

enum TOKEN_TYPE { INTEGER, MUL, DIV, PLUS, MINUS, EOF_T, LPARENT, RPARENT };

struct BaseToken {
  BaseToken(TOKEN_TYPE type) : type(type) {
  }
  TOKEN_TYPE type;
  virtual ~BaseToken() = default;
  ;
};

struct Token : BaseToken {
  Token(TOKEN_TYPE type, int val) : BaseToken(type), val(val) {
  }
  Token(TOKEN_TYPE type, int &&val) : BaseToken(type), val(val) {
  }

  int val;
  ~Token() override = default;
  ;
};

class Lexer {
 private:
  std::string text{};
  size_t pos{};
  char cur_char{};

 public:
  void advance();
  void skip_whitespace();
  int integer();
  Lexer(const std::string &text_in) : text(text_in) {
    cur_char = text_in[0];
  };

  std::unique_ptr<BaseToken> get_next_token();
};

#endif  // PARSER_LEXER_H
