//
// Created by vlad on 14.10.2019.
//
#include <iostream>
#include "Interpreter.h"
#include "Lexer.h"
#include "TestRunner.h"

int InterpretString(const std::string& inp) {
    Lexer le(inp);
    Interpreter intrp(le);
    return intrp.Expr();
}

void BaseTest() {
    std::string inp = "2 + 3 * 4 - -2";
    ASSERT_EQUAL(InterpretString(inp), 16);
}

void InvalidInp() {
    std::string inp = "2 + b";
    try {
        InterpretString(inp);
    } catch (std::logic_error& e) {
        ASSERT(true);
    } catch (std::exception& e) {
        ASSERT(false);
    }
}

void TestSum() {
    std::string inp_1 = "2 + 3+2";
    std::string inp_2 = "2 + 1";
    std::string inp_3 = "2 + -1";
    std::string inp_4 = "2 - -1";

    ASSERT_EQUAL(InterpretString(inp_1), 7);
    ASSERT_EQUAL(InterpretString(inp_2), 3);
    ASSERT_EQUAL(InterpretString(inp_3), 1);
    ASSERT_EQUAL(InterpretString(inp_4), 3);
}

void TestUnary() {
    std::string inp_1 = "-5";
    std::string inp_2 = "--5";
    ASSERT_EQUAL(InterpretString(inp_1), -5);
    ASSERT_EQUAL(InterpretString(inp_2), 5);
}

void TestCommut() {
    std::string inp_1 = "5+2*-10";
    std::string inp_2 = "5*2-10";
    ASSERT_EQUAL(InterpretString(inp_1), -15);
    ASSERT_EQUAL(InterpretString(inp_2), 0);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, InvalidInp);
    RUN_TEST(tr, TestSum);
    RUN_TEST(tr, BaseTest);
    RUN_TEST(tr, TestUnary);
    RUN_TEST(tr, TestCommut);
    return 0;
}