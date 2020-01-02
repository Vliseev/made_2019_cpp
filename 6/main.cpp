#include <iostream>
#include "../test_runner.h"
#include "Bigint.h"

void Test1() {
    Bigint b("122676119272915029368");
    Bigint a("63449541011729273910");
    Bigint res("59226578261185755458");
    Bigint c;

    c = b - a;
    ASSERT_EQUAL(c, res);

    a = Bigint("-63449541011729273910");
    res = Bigint("186125660284644303278");

    c = b - a;
    ASSERT_EQUAL(c, res);

    a = Bigint("340282366920938463463374607431768211455");
    b = Bigint("340282366920938463463374607431768211455");
    res = Bigint("680564733841876926926749214863536422910");

    c = b + a;
    ASSERT_EQUAL(c, res);

    b = Bigint("-340282366920938463463374607431768211455");
    res = Bigint("0");

    c = b + a;

    ASSERT_EQUAL(c, res);
}

void Test3() {
    Bigint b(122676);
    Bigint a("-63449541011729273910");
    Bigint res("63449541011729396586");
    Bigint c;

    c = b - a;
    ASSERT_EQUAL(c, res);
    b = 122676;
    c = b - a;
    ASSERT_EQUAL(c, res);
    c = -a + 122676;
    ASSERT_EQUAL(c, res);
    c = -a;
    c += 122676;
    ASSERT_EQUAL(c, res);

    c = 122676 - a;
    ASSERT_EQUAL(c, res);
    c = a + (-122676);

    res = Bigint("-63449541011729396586");
    ASSERT_EQUAL(c, res);
    c = a;
    c += (-122676);

    ASSERT_EQUAL(c, res);

    b = Bigint(-122676);
    c = a + b;

    res = Bigint("-63449541011729396586");
    ASSERT_EQUAL(c, res);

    a = Bigint("63449541011729273910");
    c = a + (-122676);

    res = Bigint("63449541011729151234");
    ASSERT_EQUAL(c, res);

    c = a;
    c += (-122676);

    res = Bigint("63449541011729151234");
    ASSERT_EQUAL(c, res);

    c = a - (-122676);

    res = Bigint("63449541011729396586");
    ASSERT_EQUAL(c, res);

    a = Bigint("-63449541011729273910");
    c = a - (-122676);
    res = Bigint("-63449541011729151234");
    ASSERT_EQUAL(c, res);

    a = Bigint("-6344");
    c = a - (-122676);
    res = Bigint("116332");
    ASSERT_EQUAL(c, res);

    c = (122676) + a;
    ASSERT_EQUAL(c, res);

    c = a;
    c -= -122676;
    ASSERT_EQUAL(c, res);

    a = Bigint("000");
    c = a - (122676);
    res = Bigint("-122676");
    ASSERT_EQUAL(c, res);

    a = Bigint("000");
    c = a;
    c += (122676);
    res = Bigint("122676");
    ASSERT_EQUAL(c, res);

    c = a;
    c -= (122676);
    res = Bigint("-122676");
    ASSERT_EQUAL(c, res);

    a = Bigint("100");
    c = a;
    c -= (-122676);
    res = Bigint("122776");
    ASSERT_EQUAL(c, res);

    c = -122676;
    bool is_eq = (c == -122676);
    ASSERT(is_eq);
    is_eq = (-c == 122676);
    ASSERT(is_eq);

    a = Bigint("63449541011729396586");
    c = a;
    c *= 100000;
    res = Bigint("6344954101172939658600000");
    ASSERT_EQUAL(c, res);

    c = a;
    c *= -100000;
    res = Bigint("-6344954101172939658600000");
    ASSERT_EQUAL(c, res);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, Test3);

    return 0;
}