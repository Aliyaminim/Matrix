#include <iostream>
#include "Matrix.hpp"
#include "doublecomp.hpp"

using namespace yLab;

int main() {
    int size;
    std::cin >> size;
    assert(std::cin.good());

    Matrix::Matrix<double> m{size, size};

    std::cin >> m;
    double det = m.determ();

    std::cout << det;
    #ifdef DEBUG
    double right_det;
    std::cin >> right_det;
    assert(std::cin.good());

    if (cmp::equal(right_det, det))
        std::cout << " : OK";
    else 
        std::cout << " : FAIL " << right_det;
    #endif
    std::cout << std::endl;

    return 0;
}