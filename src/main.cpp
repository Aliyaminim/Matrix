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

    double right_det;
    std::cin >> right_det;
    assert(std::cin.good());

    if (cmp::equal(right_det, det))
        std::cout << det << " : OK" << std::endl;
    else 
        std::cout << det << " : FAIL " << right_det << std::endl;
    return 0;
}