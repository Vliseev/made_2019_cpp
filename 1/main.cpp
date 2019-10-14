// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Interpreter.h"
#include "Lexer.h"
#include <iostream>
#include <functional>

int main() {
  using namespace std;

  std::string s = "5----5";
  Lexer le(s);
  Interpreter intrp(&le);
  cout<< intrp.expr();

  return 0;
}