#include "gtest/gtest.h"
#include "Matrix.hpp"
#include <iostream>
#include <random>

using namespace yLab;

TEST(Test_swap, test1) {
    Matrix::Matrix<double> m{10, 10, 8};
    m[7][8] = 3;
    m.swap_columns(0, 8);
    m.swap_rows(0, 7);
    ASSERT_TRUE(m[0][0] == 3);
}

TEST(Test_swap, test2) {
    Matrix::Matrix<double> m{20, 20, 8};
    m[0][0] = 3;
    m.swap_columns(0, 0);
    m.swap_rows(0, 0);
    ASSERT_TRUE(m[0][0] == 3);
}

TEST(Test_ctor, test3) {
    Matrix::Matrix<double> m{20, 20, 8};
    m[0][0] = 3;
    m.swap_columns(0, 0);
    m.swap_rows(0, 0);
    ASSERT_TRUE(m[0][0] == 3);
}



TEST(Test_ctor, copy_ctor) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-20, 80);

    int size = 10;
    Matrix::Matrix<int> m1{size, size};
    for(int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            m1[i][j] = distribution_int(gen);
        }
    
    Matrix::Matrix<int> m2 = m1;

    for(int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            ASSERT_TRUE(m1[i][j] == m2[i][j]);
        }
}


TEST(Max_Submatrix_Element, test1) {
    Matrix::Matrix<double> m{500, 500};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-20, 80);

    int right_max = -11;
    for(int i = 0; i < 500; ++i)
        for (int j = 0; j < 500; ++j) {
            m[i][j] = distribution_int(gen);
        }
    for(int i = 0; i < 500; ++i)
    for(int j = 0; j < 500; ++j)
        if (m[i][j] > right_max)
            right_max = m[i][j];

    auto tmp = m.max_submatrix_element(0);
    int res = *(std::get<0>(tmp));
    ASSERT_TRUE(right_max == res);
}

TEST(Max_Submatrix_Element, test2) {
    Matrix::Matrix<double> m{600, 600};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-20, 90);

    int right_max = -11;
    for(int i = 0; i < 600; ++i)
        for (int j = 0; j < 600; ++j) {
            m[i][j] = distribution_int(gen);
            if (m[i][j] > right_max)
                right_max = m[i][j];
        }
    auto tmp = m.max_submatrix_element(0);
    ASSERT_TRUE(right_max == *(std::get<0>(tmp)));
}

TEST(Test_eliminate, test1) {
    Matrix::Matrix<double> m{600, 600};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-10, 10);

    for(int i = 0; i < 600; ++i)
        for (int j = 0; j < 600; ++j) {
            m[i][j] = distribution_int(gen);
        }
    m.eliminate(0);

    //std::cout << m[5][0];
    ASSERT_TRUE(cmp::is_zero(m[5][0]));
}

