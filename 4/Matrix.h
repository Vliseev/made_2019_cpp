//
// Created by user on 10.11.2019.
//

#pragma once
#include <cstddef>
#include <stdexcept>
class Matrix {
    size_t n_row_;
    size_t n_col_;
    int *data_ = nullptr;

 public:
    Matrix(size_t n_row, size_t n_col);
    ~Matrix();
    size_t GetRows() const;
    size_t GetColumns() const;
    bool operator==(const Matrix &rhs) const;
    bool operator!=(const Matrix &rhs) const;
    Matrix &operator*=(int val);

    struct RowProxy {
        Matrix &m_;
        size_t row_;
        explicit RowProxy(Matrix &m, size_t row) : m_(m), row_(row) {}
        int &operator[](size_t col);
    };

    struct ConstRowProxy {
        const Matrix &m_;
        size_t row_;
        explicit ConstRowProxy(const Matrix &m, size_t row)
            : m_(m), row_(row) {}
        const int &operator[](size_t col) const;
    };

    RowProxy operator[](size_t row);
    ConstRowProxy operator[](size_t row) const;
};
