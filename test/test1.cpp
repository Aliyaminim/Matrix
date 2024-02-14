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

TEST(Test_swap, test3) {
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

    int right_max = -21;
    for(int i = 0; i < 500; ++i)
        for (int j = 0; j < 500; ++j) 
            m[i][j] = distribution_int(gen);
    
    int sub_idx = 0;
    for(int i = sub_idx; i < 500; ++i)
        if (m[sub_idx][i] > right_max)
            right_max = m[sub_idx][i];

    auto tmp = m.max_submatrix_element(sub_idx);
    int res = *(std::get<0>(tmp));
    ASSERT_TRUE(right_max == res);
}

TEST(Max_Submatrix_Element, test2) {
    Matrix::Matrix<long long> m{600, 600};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-20, 90);

    int right_max = -21;
    for(int i = 0; i < 600; ++i)
        for (int j = 0; j < 600; ++j) {
            m[i][j] = distribution_int(gen);
        }

    int sub_idx = 104;
    for(int i = sub_idx; i < 500; ++i)
        if (m[sub_idx][i] > right_max)
            right_max = m[sub_idx][i];

    auto tmp = m.max_submatrix_element(sub_idx);
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

    for (int i = 1; i < 600; ++i)
        ASSERT_TRUE(cmp::is_zero(m[i][0]));
}

TEST(Test_pivot, test1) {
    Matrix::Matrix<int> m{10, 10};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-10, 10);

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j) {
            m[i][j] = distribution_int(gen);
        }
    
    m[5][0] = 200;

    auto [position, pivot] = m.find_pivot(0);
    ASSERT_TRUE(pivot == m[5][0]);
}

TEST(Test_transpose, test1) {
    Matrix::Matrix<int> m{10, 10};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-100, 100);

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j) {
            m[i][j] = distribution_int(gen);
        }
    
    Matrix::Matrix<int> m2 = m;
    m2.transpose();
    m.transpose();

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j) {
            ASSERT_TRUE(m[i][j] == m2[i][j]);
        }
}

TEST(Determinant, test1) {
    Matrix::Matrix<int> m{10, 10};
    for(int i = 0; i < 10; ++i)
        for(int j = i; j < 10; ++j) {
            m[i][j] = 1;
        }
    m[9][9] = 10;
    ASSERT_TRUE(m.determ() == 10);
}

TEST(Determinant, test2) {
    Matrix::Matrix<unsigned long long> m{5, 5};
    for(int i = 0; i < 5; ++i)
        for(int j = i; j < 5; ++j) {
            m[i][j] = 1;
        }
    m[4][4] = 10;
    ASSERT_TRUE(m.determ() == 10);
}

TEST(Determinant, test3) {
    Matrix::Matrix<double> m{10, 10};
    for(int i = 0; i < 10; ++i)
        for(int j = i; j < 10; ++j) {
            m[i][j] = 1;
        }
    m[9][9] = 0.5;
    ASSERT_TRUE(m.determ() == 0.5);
}