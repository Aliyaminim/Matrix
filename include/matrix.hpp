#pragma once

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

//float elems, exceptions
//virtual dtor
//size is int
//может отнаследоваться как частный случай в квадратную матрицу

namespace Matrix {

class Matrix {
protected:
    int cols, rows;

    struct ProxyRow {
        float *row;
        //constructor
        ProxyRow(int cols_): row(new float[cols_]){};
        ProxyRow() = default;
        const float& operator[](int n) const { return row[n]; }
        float& operator[](int n) { return row[n]; }
    };

    ProxyRow* arr;

public:
    // конструктор для создания матрицы, заполненной значением
    Matrix(int cols_, int rows_, float val = 0) : cols(cols_), rows(rows_), arr(new ProxyRow[rows_]) {
        for (int i = 0; i < rows_; ++i) {
            arr[i] = ProxyRow{cols_};
            for (int j = 0; j < cols_; ++j) 
                arr[i][j] = val;
        }
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

    virtual ~Matrix() {
        //contoversial
        for (int i = 0; i < rows; ++i)
            delete [] arr[i].row;
        delete [] arr; 
    }

public: //operators' overloading

    const ProxyRow& operator[](int n) const {
        return arr[n];
    }

    ProxyRow& operator[](int n) {
        return arr[n];
    }

    /*Quat operator-() const {
return Quat{-x, -y, -z, -w};*/
//}

private:
    /*using ElemPtr = float*;

    ElemPtr max_submatrix_element(int curr_index) {
        Elemptr res = arr[curr_index]
        for (int i = curr_index; i < )
            for (int j = curr_index, j < size)
                if (fabs(arr[i][j]) > fabs([]))
    }*/


}; //class

class Square_Matrix : public Matrix {

public:
    //ctor
    Square_Matrix(int size) : Matrix(size,size) {};
    ~Square_Matrix() = default;

    bool valid() const { return cols == rows; };

    // "конструктор" для создания единичной матрицы
    static Square_Matrix eye(int size) {
        Square_Matrix m{size};
        for (int i = 0; i < size; ++i)
            m[i][i] = 1;
        
        return m;
    }

public:

}; //class
} //namespace