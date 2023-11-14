#include <iostream>
#include "../include/matrix.hpp"

int main() {
    Matrix::Matrix m{2, 2};
    Matrix::Matrix m1 = m;

    std::cin >> m;
    //auto p = m.max_submatrix_element(0);

    std::cout << m.determ() << "\n" << m << std::endl;
    return 0;
}