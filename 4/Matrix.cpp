//
// Created by user on 10.11.2019.
//

#include "Matrix.h"
#include <algorithm>
Matrix::Matrix(size_t n_row, size_t n_col)
    : n_row_(n_row), n_col_(n_col), data_(new int[n_col * n_row]) {}

size_t Matrix::GetRows() const {
    return n_row_;
}
size_t Matrix::GetColumns() const {
    return n_col_;
}
bool Matrix::operator==(const Matrix& rhs) const {
    return n_row_ == rhs.n_row_ && n_col_ == rhs.n_col_ &&
           std::equal(data_, data_ + n_col_ * n_row_, rhs.data_);
}
bool Matrix::operator!=(const Matrix& rhs) const {
    return !(rhs == *this);
}
Matrix& Matrix::operator*=(int val) {
    std::for_each(data_, data_ + n_row_ * n_col_,
                  [val](int el) -> int { return el + val; });
    return *this;
}

Matrix::~Matrix() {
    delete[] data_;
}

Matrix::RowProxy Matrix::operator[](size_t row) {
    if (row >= n_col_)
        throw std::out_of_range("");
    return Matrix::RowProxy(*this, row);
}

Matrix::ConstRowProxy Matrix::operator[](size_t row) const {
    return Matrix::ConstRowProxy(*this, row);
}

int& Matrix::RowProxy::operator[](size_t col) {
    if (col >= m_.n_col_)
        throw std::out_of_range("");
    return *(m_.data_ + row_ * m_.n_col_ + col);
}

const int& Matrix::ConstRowProxy::operator[](size_t col) const {
    if (col >= m_.n_col_)
        throw std::out_of_range("");
    return *(m_.data_ + row_ * m_.n_col_ + col);
}
