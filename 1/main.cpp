// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <iostream>
#include "Interpreter.h"
#include "Lexer.h"

int main(int argc, char* argv[]) {
    using namespace std;
    std::string s;
    try {
        s = argv[1];
        Lexer le(s);
        Interpreter intrp(le);
        cout << intrp.Expr() << endl;
    } catch (std::exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}