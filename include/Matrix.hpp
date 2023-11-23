#include <iostream>
#include <new>

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
    // rule of five
    //
    //

    explicit ProxyRow(int cols_ = 0) : row(new T[cols_]), cols(cols_) {} //may throw bad_alloc

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
public: //protected
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

    explicit MatrixBuf(int rows_ = 0, int cols_ = 0) {
        if ((rows != 0) && (cols != 0)) {
            arr = static_cast<ProxyRow<T>*>(::operator new(sizeof(ProxyRow<T>)*rows_));
            try {
                for (int i = 0; i < rows_; ++i) 
                    arr[i] = ProxyRow<T>{cols_};
            }
            catch(const std::bad_alloc &e) {
                delete [] arr; 
                throw;
            }
            rows = rows_;
            cols = cols_;
        }
    }

    ~MatrixBuf() {
        destroy(arr, arr + rows);
        ::operator delete[](arr);
    }
};

/*template<typename T> class Matrix: private MatrixBuf {
    using MatrixBuf<T>::arr;
    using MatrixBuf<T>::rows;
    using MatrixBuf<T>::cols;

    explicit

};*/ //class
} //namespaces
}