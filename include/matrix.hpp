#pragma once

#include <iostream>
#include <cassert>
#include <cstdlib>

//float elems

namespace Matrix {

class Matrix {
private:
    int cols, rows;
    float** arr; //jagged array

    struct ProxyRow {
        float *row;
        //constructor
        ProxyRow(float* ptr = nullptr): row(ptr) {};
        const float& operator[](int n) const { return row[n]; }
        float& operator[](int n) { return row[n]; }
    };

    struct ProxyCol {
        float *col;
        //constructor
        ProxyCol(float* ptr = nullptr): col(ptr) {};
        const float& operator[](int n) const { return col[n]; }
        float& operator[](int n) { return col[n]; }
    };

public:
    // конструктор для создания матрицы, заполненной значением
    Matrix(int cols_, int rows_, float val = 0) : cols(cols_), rows(rows_) {
        arr = new float*[rows_];
        for (int i = 0; i < rows_; ++i) {
            arr[i] = new float[cols_];
            for (int j = 0; j < cols_; ++j) 
                arr[i][j] = val;
        }
    }

    // "конструктор" для создания единичной матрицы
    static Matrix eye(int cols_, int rows_) {
        assert((cols_ == rows_) && "eye matrix is quadratic");
        Matrix m{cols_, rows_};
        for (int i = 0; i < rows_; ++i)
            m[i][i] = 1;
        
        return m;
    }

public: //big five

    // копирующий и перемещающий конструктор
    Matrix(const Matrix &rhs) {
        cols = rhs.cols;
        rows = rhs.rows;
        arr = new float*[rows];
        for (int i = 0; i < rows; ++i) {
            arr[i] = new float[cols];
            for (int j = 0; j < cols; ++j) 
                arr[i][j] = rhs.arr[i][j];
        }
    }

    Matrix(Matrix &&rhs) { 
        //check if rhs doesn't equal this
        //ready to die, but rhs remains consistent
        cols = rhs.cols;
        rows = rhs.rows;
        arr = rhs.arr;

        rhs.arr = nullptr;
    }

    // присваивание и перемещение
    Matrix& operator=(const Matrix &rhs) {
        cols = rhs.cols;
        rows = rhs.rows;
        arr = new float*[rows];
        for (int i = 0; i < rows; ++i) {
            arr[i] = new float[cols];
            for (int j = 0; j < cols; ++j) 
                arr[i][j] = rhs.arr[i][j];
        }
        return *this;
    }

    Matrix& operator=(Matrix &&rhs) {
        //check if rhs doesn't equal this
        cols = rhs.cols;
        rows = rhs.rows;
        arr = rhs.arr;
        rhs.arr = nullptr;

        return *this;
    }

    ~Matrix() {
        //contoversial
        for (int i = 0; i < rows; ++i)
            delete [] arr[i];
        delete [] arr; 
    }

public: //operators' overloading

    const ProxyRow& operator[](int n) const {
        const ProxyRow row{arr[n]};
        return row;
    }

    ProxyRow& operator[](int n) {
        ProxyRow row{arr[n]};
        return row;
    }
 


}; //class
} //namespace