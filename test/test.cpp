#include <iostream>
#include "Matrix.hpp"
#include <random>

using namespace yLab;

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_int(-10, 10);
    std::uniform_real_distribution<double> distribution_real(-20.0, 20.0);

    std::cout << "Testing ctors: " << std::endl;

    for (int k = 1; k < 5; k++) {
        
        Matrix::Matrix<int> matrix1{5, 5};
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                matrix1[i][j] = distribution_int(gen);
            }
        }
        Matrix::Matrix<int> matrix2 = matrix1;  
        std::cout << "Test" << k << " ";
        std::string res = "OK";
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (matrix1[i][j] != matrix2[i][j])
                    res = "FAIL";
            }
        }
        std::cout << res << std::endl;


    }

 /*   std::cout << "Input tests" << std::endl;
    for (int i = 1; i < 5; i++){
        matrix_template::Matrix<double> matrix1(i, 0);
        matrix_template::Matrix<double> matrix2(0, i);
        matrix_template::Matrix<double> matrix3(0, 0);
    }

    std::cout << "Test for intenger numbers:" << std::endl;
    std::uniform_int_distribution<int> distribution_int(-10, 10);

    for (size_t kol = 1; kol < 100; kol++){
        matrix_template::Matrix<int> matrix1(100,100);
        double det_correct1 = 1.0;

        for (size_t j = 0; j < 99; j++){
            for (size_t k = 0; k < 99; k++ ){
                if (j == k) {
                    matrix1[j][k] = distribution_int(gen);
                    det_correct1 = det_correct1 * matrix1[j][k];
                }
                else matrix1[j][k] = 0;
            }
        }

        for (size_t i = 0; i < 99; i++) {
            matrix1[i+1] = matrix1[i+1] + matrix1[i];
        }

        for (size_t i = 0; i < 99; i--){

            matrix1[i] = matrix1[i] + matrix1[99] * 2 ;
        }

        double det1 = matrix1.determinant() ;
        if (det_correct1 == 0){
            if (det1 == 0){
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            }
            else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct1 << "Result of the program =" << " "
                          << det1 << std::endl;

            }
        }
        else {
            double result = det1 / det_correct1;
            if ((result - 1) < 0.000001) {
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            } else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct1 << "Result of the program =" << " "
                          << det1 << std::endl;

            }
        }

    }

    std::cout << std::endl;

    std::cout << "Test for real numbers:" << std::endl;
    std::uniform_real_distribution<double> distribution_real(-20.0, 20.0);

    for (size_t kol = 1; kol <= 100; kol++){
        matrix_template::Matrix<double> matrix2(100,100);
        double det_correct2 = 1.0;

        for (size_t j = 0; j < 100; j++){
            for (size_t k = 0; k < 100; k++ ){
                if (j == k) {
                    matrix2[j][k] = distribution_real(gen);
                    det_correct2 = det_correct2 * matrix2[j][k];
                }
                else matrix2[j][k] = 0.0;
            }
        }

        for (size_t i = 0; i < 99; i++){
            matrix2[i+1] = matrix2[i+1] + matrix2[i];
        }

        for (size_t i = 0; i < 99; i--){

            matrix2[i] = matrix2[i] + matrix2[99] * 2 ;
        }

        double det2 = matrix2.determinant() ;
        if (det_correct2 == 0){
            if (det2 == 0){
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            }
            else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct2 << "Result of the program =" << " "
                          << det2 << std::endl;

            }
        }
        else {
            double result = det2 / det_correct2;
            if ((result - 1) < 0.000001) {
                std::cout << "Test" << " " << kol << " " << "passed" << std::endl;
            } else {
                std::cout << "Test" << " " << kol << " " << "failed. Expected answer ="
                          << " " << det_correct2 << "Result of the program =" << " "
                          << det2 << std::endl;

            }
        }
    }

*/
    return 0;
}