#pragma once

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <tuple>
#include "doublecomp.hpp"

namespace yLab {
namespace Matrix {

class Matrix final {
protected:
    int cols, rows;

    struct ProxyRow {
        double *row;
        //constructor
        ProxyRow(int cols_): row(new double[cols_]){};
        ProxyRow() = default;
        const double& operator[](int n) const { return row[n]; }
        double& operator[](int n) { return row[n]; }
    };

    ProxyRow* arr;

public:
    // конструктор для создания матрицы, заполненной значением
    Matrix(int cols_, int rows_, double val = 0) : cols(cols_), rows(rows_), arr(new ProxyRow[rows_]) {
        for (int i = 0; i < rows_; ++i) {
            arr[i] = ProxyRow{cols_};
            for (int j = 0; j < cols_; ++j) 
                arr[i][j] = val;
        }
    }

    // "конструктор" для создания единичной матрицы
    static Matrix eye(int size) {
        Matrix m{size, size};
        for (int i = 0; i < size; ++i)
            m[i][i] = 1;

        return m;
    }

public: //big five

    // копирующий и перемещающий конструктор
    Matrix(const Matrix &rhs): cols(rhs.cols), rows(rhs.rows), arr(new ProxyRow[rhs.rows]) {
        for (int i = 0; i < rhs.rows; ++i) {
            arr[i] = ProxyRow{rhs.cols};
            std::copy(arr[i].row, arr[i].row + rhs.cols, rhs.arr[i].row);
        }
    }

    Matrix(Matrix &&rhs) noexcept: cols(rhs.cols), rows(rhs.rows), arr(rhs.arr) { 
        rhs.arr = nullptr;
    }

    // присваивание и перемещение
    Matrix& operator=(const Matrix &rhs) {
        if (this == &rhs) return *this;

        for (int i = 0; i < rows; ++i)
            delete [] arr[i].row; //trying to avoid user-provided destructor(rule of 5) in struct ProxyRow
        delete [] arr; 

        cols = rhs.cols;
        rows = rhs.rows;
        arr = new ProxyRow[rhs.rows];
        for (int i = 0; i < rows; ++i) {
            arr[i] = ProxyRow{rhs.cols};
            std::copy(arr[i].row, arr[i].row + rhs.cols, rhs.arr[i].row);
        }
        return *this;
    }

    Matrix& operator=(Matrix &&rhs) noexcept{
        if (this == &rhs)
            return *this;
        
        for (int i = 0; i < rows; ++i)
            delete [] arr[i].row; 
        delete [] arr; 

        cols = rhs.cols;
        rows = rhs.rows;
        arr = rhs.arr;
        rhs.arr = nullptr;

        return *this;
    }

    ~Matrix() {
        for (int i = 0; i < rows; ++i)
            delete [] arr[i].row;
        delete [] arr; 
    }

public: //operators' overloading
    const ProxyRow& operator[](int n) const { return arr[n]; }
    ProxyRow& operator[](int n) { return arr[n]; }

public: //селекторы
    int ncols() const { return cols; }
    int nrows() const { return rows; }

private:

    bool is_square() const {
        return cols == rows;
    }

    using ElemPtr = double*;

    std::tuple<ElemPtr, int, int> max_submatrix_element(const int curr_index) const{
        assert(this->is_square());

        auto res = std::make_tuple(&(arr[curr_index][curr_index]), curr_index, curr_index);
        for (int i = curr_index; i < cols; ++i)
            for (int j = curr_index; j < cols; ++j)
                if (cmp::greater(fabs(arr[i][j]), fabs(*(std::get<0>(res))))) {
                    std::get<0>(res) = &(arr[i][j]);
                    std::get<1>(res) = i;
                    std::get<2>(res) = j;
                }

        return res;
    }

    int swap_rows(const int fst, const int snd) {
        if (fst == snd) return 0;
        ProxyRow tmp = std::move(arr[fst]); //move ctor
        arr[fst] = std::move(arr[snd]); //move assign
        arr[snd] = std::move(tmp); //move assign
        return 1;
    }

    int swap_columns(const int fst, const int snd) {
        if (fst == snd) return 0;
        for (int i = fst; i < rows; ++i) {
            double tmp = arr[i][fst];
            arr[i][fst] = arr[i][snd];
            arr[i][snd] = tmp;
        }
        return 1;
    }

    void eliminate(int curr_index) {
        for (int k = curr_index + 1; k < rows; ++k) {
            double index = arr[k][curr_index] / arr[curr_index][curr_index];
            for (int m = curr_index; m < cols; ++m)
                arr[k][m] -= index * arr[curr_index][m];
        }       
    }

public:

    double determ() {
        assert(this->is_square());

        int numofswaps = 0; 
        for (int i = 0; i < rows; ++i) {
            auto el = max_submatrix_element(i);
            if (cmp::is_zero(*(std::get<0>(el))))
                return 0;
            numofswaps += swap_rows(i, std::get<1>(el));
            numofswaps += swap_columns(i, std::get<2>(el));
            
            eliminate(i);
        }

        double res = 1;
        for (int i = 0; i < rows; ++i)
            res *= arr[i][i];
        
        if (numofswaps % 2 == 1)
            res *= -1;
        
        return res;      
    }
}; //class

inline void dump (std::ostream &os, const Matrix &matrix)
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

inline std::ostream& operator<< (std::ostream &os, const Matrix &matrix) {
    dump(os, matrix);
    return os;
}

inline void read(std::istream &is, Matrix &matrix) {
    auto n_cols = matrix.ncols();
    auto n_rows = matrix.nrows();

    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            is >> matrix[i][j];
        }
    }
}

inline std::istream& operator>> (std::istream &is, Matrix &matrix) {
    read(is, matrix);
    return is;
}

} //namespace
} //namespace