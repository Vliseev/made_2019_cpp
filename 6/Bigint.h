#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <exception>
#include <ostream>
#include <stdexcept>
#include "vector.h"

namespace {
constexpr unsigned char X = 0xff;
constexpr unsigned char __gmp_digit_value_tab[] = {
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  X,  X,  X,  X,  X,  X,  X,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, X,  X,  X,  X,
    X,  X,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 33, 34, 35, X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    X,  X,  X,  X,  X,  X,  X,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, X,  X,  X,  X,  X,
    X,  36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,  X,
    X,  X,  X,  X,  X,  X,  X,  X};

constexpr int pow(int base, int pow) {
    int res = 1;
    for (auto i = 0; i < pow; ++i) {
        res *= base;
    }
    return res;
}

}    // namespace

class Bigint {
    static constexpr int Base = 10;
    static constexpr int NumDig = 9;
    static constexpr int WordMax = pow(Base, NumDig);

    Vector<int> words_;    // TODO: remove vector
    bool sign_;

    Bigint SubImpl(const Bigint& b) const;
    Bigint SumImpl(int other) const;

    void Trim();

   public:
    Bigint() : words_(), sign_(true){};
    explicit Bigint(const char* s);
    explicit Bigint(int val);
    Bigint(const Bigint& other) : words_(other.words_), sign_(other.sign_){};
    Bigint(Bigint&& other) noexcept;

    Bigint& operator=(int val);
    Bigint& operator=(const Bigint& other);
    Bigint& operator=(Bigint&& other) noexcept;

    ~Bigint() = default;

    Bigint Abs() const;
    Bigint operator+(const Bigint& other) const;
    Bigint operator+(int other) const;
    friend Bigint operator+(int, const Bigint&);
    void operator+=(int v);

    Bigint operator-(const Bigint& other) const;
    Bigint operator-(int other) const;
    Bigint operator-() const;
    friend Bigint operator-(int, const Bigint&);
    void operator-=(int v);

    void operator*=(int v);
    bool operator==(const Bigint& rhs) const;
    bool operator!=(const Bigint& rhs) const;
    bool operator<(const Bigint& rhs) const;
    bool operator>(const Bigint& rhs) const;
    bool operator<=(const Bigint& rhs) const;
    bool operator>=(const Bigint& rhs) const;

    bool operator==(int rhs) const;
    bool operator!=(int rhs) const;
    bool operator<(int rhs) const;
    bool operator>(int rhs) const;
    bool operator<=(int rhs) const;
    bool operator>=(int rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Bigint& bigint);
};
