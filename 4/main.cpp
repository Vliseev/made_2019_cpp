#include <iostream>
#include "../test_runner.h"
#include "Matrix.h"

void AssertTest() {
    Matrix m(2, 2);

    try {
        m[2];
    } catch (std::out_of_range& e) {
        ASSERT(true);
    } catch (std::exception& e) {
        ASSERT(false);
    }
    try {
        m[1][2];
    } catch (std::out_of_range& e) {
        ASSERT(true);
    } catch (std::exception& e) {
        ASSERT(false);
    }
}

void TestAccess() {
    Matrix m(5, 5);
    for (int kI = 0; kI < m.GetRows(); ++kI) {
        for (int kJ = 0; kJ < m.GetColumns(); ++kJ) {
            m[kI][kJ] = kI * m.GetColumns() + kJ;
        }
    }
    const Matrix& m_ref = m;

    for (int kI = 0; kI < m.GetRows(); ++kI) {
        for (int kJ = 0; kJ < m.GetColumns(); ++kJ) {
            ASSERT_EQUAL(m_ref[kI][kJ], kI * m_ref.GetColumns() + kJ);
        }
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, AssertTest);
    RUN_TEST(tr, TestAccess);

    return 0;
}
