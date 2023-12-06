#pragma once

#include <iostream>
#include <new>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <tuple>
#include "doublecomp.hpp"

namespace yLab { 
namespace Matrix {

struct undefined_det : public std::runtime_error {
    undefined_det(const char *message = "Determinant is not defined for non-square matrices")
              : std::runtime_error{message} {};
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

    Matrix(Matrix &&rhs) noexcept : MatrixBuf<T>(rhs) {}

    Matrix &operator=(Matrix &&rhs) = default;

    Matrix(const Matrix &rhs) : MatrixBuf<T>(rhs.rows, rhs.cols) {
        for (int i = 0; i < rows; ++i) 
        for (int j = 0; j < cols; ++j) 
            construct(arr + cols*i + j, rhs[i][j]);
    }

    Matrix &operator=(const Matrix &rhs) {
        Matrix tmp(rhs);
        std::swap(*this, tmp);
        return *this;
    }

    ~Matrix() = default;

public: //operators' overloading
    //const ProxyRow<T>& operator[](int n) const { return arr[n]; }
    //ProxyRow<T>& operator[](int n) { return arr[n]; }

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

    std::tuple<const_ElemPtr, int, int> max_submatrix_element(const int curr_idx) const{
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

//
    int swap_rows(const int fst, const int snd) noexcept {
        if (fst == snd) return 0;
        std::swap_ranges(arr + fst*cols, arr + (fst+1)*cols, arr + snd*cols);
        return 1;
    }


    int swap_columns(const int fst, const int snd) {
        if (fst == snd) return 0;
        /*for (int i = fst; i < rows; ++i) {
            T tmp = std::move(arr[i][fst]);
            arr[i][fst] = std::move(arr[i][snd]);
            arr[i][snd] = std::move(tmp);
        }*/
        return 1;
    }

    void eliminate(int curr_idx) {
        for (int k = curr_idx + 1; k < rows; ++k) {
            T index = (*this)[k][curr_idx] / (*this)[curr_idx][curr_idx];
            for (int m = curr_idx; m < cols; ++m)
                (*this)[k][m] -= index * (*this)[curr_idx][m];
        }       
    }

    double determ() {
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
        
        if (numofswaps % 2 == 1)
            res *= -1;
        
        return res;      
    }


}; //class

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