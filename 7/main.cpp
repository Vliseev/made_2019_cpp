#include <iostream>
#include "format.h"
#include "test_runner.h"

void Test1() {
    try {
        Format("{{0}+{1}={2}", "one", "two", 3);
    } catch (std::runtime_error& e) {
        ASSERT(true)
    } catch (std::exception& e) {
        ASSERT(false)
    }

    try {
        Format("{0}+{1}={2}}", "one", "two", 3);
    } catch (std::runtime_error& e) {
        ASSERT(true)
    } catch (std::exception& e) {
        ASSERT(false)
    }

    try {
        Format("{0}+{1}={3}", "one", "two", 3);
    } catch (std::runtime_error& e) {
        ASSERT(true)
    } catch (std::exception& e) {
        ASSERT(false)
    }
}

void Test2() {
    auto s = Format("{0}+{0}={1}", "one", 2);
    ASSERT_EQUAL(s, "one+one=2")

    s = Format(" {1} + {1} = {0} ", 2, "one");
    ASSERT_EQUAL(s, " one + one = 2 ")
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, Test2);

    return 0;
}