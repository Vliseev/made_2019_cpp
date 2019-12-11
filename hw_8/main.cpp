#include <iostream>
#include <vector>
#include "../test_runner.h"
#include "vector.h"

void Test1() {
    Vector<int> v(10);
    ASSERT_EQUAL(v.Size(), 10)
    v.Clear();
    ASSERT_EQUAL(v.Size(), 0)
    v.Reserve(100);
    ASSERT_EQUAL(v.Size(), 0)

    for (int i = 0; i < 20; ++i) {
        v.PushBack(i);
    }

    for (int i = 0; i < 20; ++i) {
        ASSERT_EQUAL(v[i], i);
    }
    {
        int i = 0;
        for (int& it : v) {
            ASSERT_EQUAL(it, i++)
        }

        i = 19;
        for (auto it = v.rbegin(); it != v.rend(); ++it) {
            ASSERT_EQUAL(*it, i--)
        }
    }
}

void Test2() {
    Vector<std::string> s;
    ASSERT(s.Empty())

    for (int i = 0; i < 20; ++i) {
        s.PushBack(std::to_string(i));
    }

    ASSERT(!s.Empty())

    for (int i = 0; i < 20; ++i) {
        s.PopBack();
    }

    ASSERT_EQUAL(s.Size(), 0)
    ASSERT(s.Empty())

    s.Resize(10);
    ASSERT_EQUAL(s.Size(), 10);
}

void Test3() {
    Vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2 = v;
    Vector<int> v3(std::move(v));

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQUAL(v2[i], i + 1)
        ASSERT_EQUAL(v3[i], i + 1)
    }
}

void Test4() {
    Vector<int> v;

    for (int i = 0; i < 1000; ++i) {
        v.PushBack(i);
    }
    v.Resize(10);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, Test2);
    RUN_TEST(tr, Test3);
    RUN_TEST(tr, Test4);

    return 0;
}