#include <iostream>
#include "matrix.hpp"
#include "doublecomp.hpp"

using namespace yLab;

int main() {
    int size;
    std::cin >> size;
    assert(std::cin.good());

    Matrix::Matrix m{size, size};

    std::cin >> m;
    double det = m.determ();

    int right_det;
    std::cin >> right_det;
    assert(std::cin.good());

    if (cmp::equal(right_det, det))
        std::cout << det << " : OK" << std::endl;
    return 0;
}