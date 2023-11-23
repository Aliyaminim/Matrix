#include <iostream>
#include <new>
#include <algorithm>

namespace yLab {
namespace Matrix {

//размещающий new
template <typename T> void construct(T *p, const T &rhs) { new (p) T(rhs); }
template <typename T> void construct(T *p, T &&rhs) {
    new (p) T(std::move(rhs));
}

template <class S> void destroy(S *p) { p->~S(); }
template <typename FwdPtr> void destroy(FwdPtr first, FwdPtr last) {
    while (first != last) 
        destroy(first++);
}    

template<typename T> class ProxyRow{
    T* row;
    int cols;
public:
    explicit ProxyRow(int cols_ = 0) : row(new T[cols_]), cols(cols_) {} //may throw bad_alloc

    ProxyRow(const ProxyRow &rhs) = delete;
    ProxyRow &operator=(const ProxyRow &rhs) = delete;

    ProxyRow(ProxyRow &&rhs) noexcept : row(rhs.row), cols(rhs.cols) {
        rhs.row = nullptr;
        rhs.cols = 0;
    }

    ProxyRow &operator=(ProxyRow &&rhs) noexcept {
        std::swap(row, rhs.row);
        std::swap(cols, rhs.cols);
        return *this;
    }

    ~ProxyRow() {
        delete [] row;
    }

    const double& operator[](int n) const { return row[n]; }
    double& operator[](int n) { return row[n]; }
};

template<typename T> class MatrixBuf {
protected:
    ProxyRow<T>* arr = nullptr;
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
        : arr(((rows_ != 0) && (cols_ != 0)) ? static_cast<ProxyRow<T>*>(::operator new(sizeof(ProxyRow<T>)*rows_)) : nullptr)
    {
        if ((rows_ != 0) && (cols_ != 0)) {
            for (int i = 0; i < rows_; ++i) 
                arr[i] = ProxyRow<T>{cols_};
            
            rows = rows_;
            cols = cols_;
        } else {
            rows = 0;
            cols = 0;
        }
    }

    ~MatrixBuf() {
        destroy(arr, arr + rows);
        delete arr;
    }
};

template<typename T> class Matrix : private MatrixBuf<T> {
    using MatrixBuf<T>::arr;
    using MatrixBuf<T>::rows;
    using MatrixBuf<T>::cols;

public:

    explicit Matrix(int rows_ = 0, int cols_ = 0) : MatrixBuf<T>(rows_, cols_) {}

    Matrix(Matrix &&rhs) noexcept : MatrixBuf<T>(rhs) {}

    Matrix &operator=(Matrix &&rhs) = default;

    Matrix(const Matrix &rhs) : MatrixBuf<T>(rhs.rows, rhs.cols) {
        for (int i = 0; i < rows; ++i) 
        for (int j = 0; j < cols; ++j) 
            arr[i][j] = rhs[i][j]; 
    }

    Matrix &operator=(const Matrix &rhs) {
        Matrix tmp(rhs);
        std::swap(*this, tmp);
        return *this;
    }

    ~Matrix() = default;

public: //operators' overloading
    const ProxyRow<T>& operator[](int n) const { return arr[n]; }
    ProxyRow<T>& operator[](int n) { return arr[n]; }

public: //селекторы
    int ncols() const { return cols; }
    int nrows() const { return rows; }

}; //class
} //namespaces
}