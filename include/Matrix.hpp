#pragma once

#include <iostream>
#include <new>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <tuple>
#include <concepts>
#include <type_traits>
#include "doublecomp.hpp"

namespace yLab {
namespace Matrix {

struct undefined_det : public std::runtime_error {
    undefined_det(const char *message = "Determinant is not defined for non-square matrices")
              : std::runtime_error{message} {};
};

struct undefined_product : public std::runtime_error {
    undefined_product ()
                  : std::runtime_error{"Product A*B of matrices does not exist, because A.cols != B.rows"} {};
};

template <typename T> void construct(T *p, const T &rhs) { new (p) T(rhs); }
template <typename T> void construct(T *p, T &&rhs) {
    new (p) T(std::move(rhs));
}

template <class S> void destroy(S *p) { p->~S(); }
template <typename FwdIter> void destroy(FwdIter first, FwdIter last) {
    while (first != last)
        destroy(first++);
}

template<typename T> class Proxy_Row {
    T* row;
public:
    explicit Proxy_Row(T* def_ptr): row(def_ptr){}
    const T &operator[] (int j) const { return row[j]; }
    T &operator[] (int j)  { return row[j]; }
};

template<typename T> class MatrixBuf {
protected:
    T* arr = nullptr;
    int rows = 0;
    int cols = 0;
protected:
    MatrixBuf(const MatrixBuf &) = delete;
    MatrixBuf& operator=(const MatrixBuf &) = delete;

    MatrixBuf(MatrixBuf &&rhs) noexcept
        : arr(rhs.arr), rows(rhs.rows), cols(rhs.cols) {
        rhs.arr = nullptr;
        rhs.cols = 0;
        rhs.rows = 0;
    }

    MatrixBuf& operator=(MatrixBuf &&rhs) noexcept {
        std::swap(arr, rhs.arr);
        std::swap(rows, rhs.rows);
        std::swap(cols, rhs.cols);

        return *this;
    }

    explicit MatrixBuf(int rows_ = 0, int cols_ = 0)
        : arr(((rows_ != 0) && (cols_ != 0)) ? static_cast<T*>(::operator new(sizeof(T)*rows_*cols_)) : nullptr)
    {
        if ((rows_ != 0) && (cols_ != 0)) {
            rows = rows_;
            cols = cols_;
        } else {
            rows = 0;
            cols = 0;
        }
    }

    ~MatrixBuf() {
        destroy(arr, arr + rows);
        ::operator delete (arr);
    }
};

template<typename T> class Matrix final : private MatrixBuf<T> {
    using MatrixBuf<T>::arr;
    using MatrixBuf<T>::rows;
    using MatrixBuf<T>::cols;

public:

    explicit Matrix(int rows_ = 0, int cols_ = 0, T val = T{}) : MatrixBuf<T>(rows_, cols_) {

        std::fill (arr, arr + rows_*cols_, val);
    }

    Matrix(Matrix &&rhs) noexcept = default;

    Matrix &operator=(Matrix &&rhs) = default;

    Matrix(const Matrix &rhs) : MatrixBuf<T>(rhs.rows, rhs.cols) {
        for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            construct(arr + cols*i + j, rhs[i][j]);
    }

    Matrix &operator=(const Matrix &rhs) {
        Matrix tmp(rhs);
        //kalb line
        std::swap(*this, tmp);
        return *this;
    }

    ~Matrix() = default;

public: //operators' overloading

    const Proxy_Row<T> operator[] (int row_i) const
    {
        return Proxy_Row<T>{arr + row_i * cols};
    }

    Proxy_Row<T> operator[] (int row_i)
    {
        return Proxy_Row<T>{arr + row_i * cols};
    }

public:
    int ncols() const { return cols; }
    int nrows() const { return rows; }

    bool is_square() const {
        return cols == rows;
    }

    using ElemPtr = T*;
    using const_ElemPtr = const T*;

    int swap_rows(const int fst, const int snd) noexcept {
        if (fst == snd) return 0;
        std::swap_ranges(arr + fst*cols, arr + (fst+1)*cols, arr + snd*cols);
        return 1;
    }


    int swap_columns(const int fst, const int snd) {
        if (fst == snd) return 0;
        for (int i = 0; i < rows; ++i) {
            T tmp = std::move((*this)[i][fst]);
            (*this)[i][fst] = std::move((*this)[i][snd]);
            (*this)[i][snd] = std::move(tmp);
        }
        return 1;
    }

    void eliminate(int curr_idx) {
        for (int k = curr_idx + 1; k < rows; ++k) {
            T index = (*this)[k][curr_idx] / (*this)[curr_idx][curr_idx];
            for (int m = curr_idx; m < cols; ++m)
                (*this)[k][m] -= index * (*this)[curr_idx][m];
        }
    }

    // Gauss algorithm
    double det_gauss() {
        if (!is_square())
            throw undefined_det{};

        int numofswaps = 0;
        for (int i = 0; i < rows; ++i) {
            auto el = max_submatrix_element(i);
            auto pivot = *(std::get<0>(el));
            if (pivot == T{})
                return pivot;

            numofswaps += swap_rows(i, std::get<1>(el));
            numofswaps += swap_columns(i, std::get<2>(el));

            eliminate(i);
        }

        double res = 1;
        for (int i = 0; i < rows; ++i)
            res *= (*this)[i][i];

        if (cmp::is_zero(res))
            res = 0.0;

        return (numofswaps % 2 == 1) ? -res : res;
    }

    // Bareiss algorithm
    double det_bareiss () {
        if (!is_square())
            throw undefined_det{};
        int numofswaps = 0;
        for (int k = 0; k < cols - 1; k++) {
            auto [position, pivot] = find_pivot(k);
            if (pivot == T{})
                return pivot;
            else {
                numofswaps += swap_rows(k, position);
                for (int i = k + 1; i < cols; i++) {
                for (int j = k + 1; j < cols; j++) {
                    (*this)[i][j] = (*this)[k][k] * (*this)[i][j] - (*this)[i][k] * (*this)[k][j];
                    if(k != 0) {
                        (*this)[i][j] /= (*this)[k-1][k-1];
                    }
                }
                }
            }
        }
        double res = (*this)[rows-1][cols-1];
        return (numofswaps % 2 == 1) ? -res : res;
    }

    double determ() {
        if (!is_square())
            throw undefined_det{};

        if constexpr (std::unsigned_integral<T>) {
            Matrix m = *this;
            m.convert_to_double();
            return m.det_gauss();
        } else if (std::signed_integral<T>) {
            Matrix m = *this;
            return m.det_bareiss();
        } else {
            Matrix m = *this;
            return m.det_gauss();
        }
    }

    std::tuple<const_ElemPtr, int, int> max_submatrix_element(const int curr_idx) const {
        if (!is_square())
            throw undefined_det{};

        auto res = std::make_tuple(&((*this)[curr_idx][curr_idx]), curr_idx, curr_idx);
        for (int j = curr_idx; j < cols; ++j)
                if (cmp::greater(fabs((*this)[curr_idx][j]), fabs(*(std::get<0>(res))))) {
                    std::get<0>(res) = &((*this)[curr_idx][j]);
                    std::get<1>(res) = curr_idx;
                    std::get<2>(res) = j;
                }

        return res;
    }

    std::pair<int, T> find_pivot (int curr_row) const {
        int col = curr_row;
        std::pair<int, T> pivot {curr_row, (*this)[curr_row][col]};

        for (int i = curr_row + 1; i < cols; i++)
        {
            auto elem = (*this)[i][col];
            if (std::abs (pivot.second) < std::abs (elem))
            {
                pivot.first  = i;
                pivot.second = elem;
            }
        }

        return pivot;
    }

    void convert_to_double() {
        for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            (*this)[i][j] = static_cast<double>((*this)[i][j]);
        }
        }
    }

public:

    Matrix& transpose() & {
        if (!is_square())
            throw undefined_det{"Cannot transpose non-square matrix"};
        for (int i = 0; i < cols; i++) {
            for (int j = i + 1; j < cols; j++) {
                T tmp = std::move((*this)[i][j]);
                (*this)[i][j] = std::move((*this)[j][i]);
                (*this)[j][i] = std::move(tmp);
            }
        }

        return *this;
    }

}; //class

template<typename T>
Matrix<T> product (const Matrix<T> &lhs, const Matrix<T> &rhs) {
    if (lhs.ncols() != rhs.nrows())
        throw undefined_product {};

    Matrix<T> product{lhs.nrows(), rhs.ncols()};
    int dim = lhs.ncols();

    Matrix<T> rhs_copy = rhs;
    rhs_copy.transpose();

    for (int i = 0; i < lhs.nrows(); ++i)
        for (int j = 0; j < rhs.ncols(); ++j)
            for (int k = 0; k < dim; ++k)
                product[i][j] += lhs[i][k] * rhs_copy[j][k];

    return product;
}

template<typename T>
inline void dump (std::ostream &os, const Matrix<T> &matrix)
{
    auto n_cols = matrix.ncols();
    auto n_rows = matrix.nrows();

    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            os << matrix[i][j] << " ";
        }
        os << std::endl;
    }
}

template<typename T>
inline std::ostream& operator<< (std::ostream &os, const Matrix<T> &matrix) {
    dump(os, matrix);
    return os;
}

template<typename T>
inline void read(std::istream &is, Matrix<T> &matrix) {
    auto n_cols = matrix.ncols();
    auto n_rows = matrix.nrows();

    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            is >> matrix[i][j];
        }
    }
}

template<typename T>
inline std::istream& operator>> (std::istream &is, Matrix<T> &matrix) {
    read(is, matrix);
    return is;
}

} //namespaces
}