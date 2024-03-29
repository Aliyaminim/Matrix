#include "gtest/gtest.h"
#include "Matrix.hpp"
#include <iostream>
#include <random>

using namespace yLab;

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

TEST(Determinantm, test4) {
    Matrix::Matrix<long long> m{5, 5};
    for(int i = 1; i < 5; ++i)
        m[i][i] = 1;
    ASSERT_TRUE(m.determ() == 0);
}