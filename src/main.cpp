#include <iostream>
#include "../include/matrix.hpp"

using namespace yLab;

int main() {
    int size;
    std::cin >> size;
    assert(std::cin.good());

    Matrix::Matrix m{size, size};

    std::cin >> m;
    std::cout << m.determ() << "\n" << m << std::endl;
    return 0;
}