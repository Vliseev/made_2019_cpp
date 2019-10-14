//
// Created by vlad on 14.10.2019.
//
#include <iostream>
#include "Interpreter.h"
#include "Lexer.h"
#include "TestRunner.h"

int InterpretString(const std::string& inp) {
  Lexer le(inp);
  Interpreter intrp(&le);
  return intrp.expr();
}

void BaseTest() {
  using namespace std;
  string inp = "2 + 3 * 4 - -2";
  ASSERT_EQUAL(InterpretString(inp), 16);
}

void InvalidInp() {
  using namespace std;
  string inp = "2 + b";
  try {
    InterpretString(inp);
  } catch (std::exception& e) {
    ASSERT(typeid(e) == typeid(std::logic_error));
  }
}

void TestSum() {
  using namespace std;
  string inp1 = "2 + 3+2";
  string inp2 = "2 + 1";
  string inp3 = "2 + -1";
  string inp4 = "2 - -1";

  ASSERT_EQUAL(InterpretString(inp1), 7);
  ASSERT_EQUAL(InterpretString(inp2), 3);
  ASSERT_EQUAL(InterpretString(inp3), 1);
  ASSERT_EQUAL(InterpretString(inp4), 3);
}

void TestUnary() {
  using namespace std;
  string inp1 = "-5";
  string inp2 = "--5";
  ASSERT_EQUAL(InterpretString(inp1), -5);
  ASSERT_EQUAL(InterpretString(inp2), 5);
}

void TestCommut() {
  using namespace std;
  string inp1 = "5+2*-10";
  string inp2 = "5*2-10";
  ASSERT_EQUAL(InterpretString(inp1), -15);
  ASSERT_EQUAL(InterpretString(inp2), 0);
}

int main() {
  using namespace std;

  TestRunner tr;
  RUN_TEST(tr, InvalidInp);
  RUN_TEST(tr, TestSum);
  RUN_TEST(tr, BaseTest);
  RUN_TEST(tr, TestUnary);
  RUN_TEST(tr, TestCommut);
  return 0;
}