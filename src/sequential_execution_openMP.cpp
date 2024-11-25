// sequential_execution_openMP.cpp

#include <iostream>
#include <vector>
#include <omp.h>
#include <fstream>
#include "matrix_operations_openMP.h"
#include <time.h>

double compute_error(const Matrix& result, const Matrix& expected_result, int N) {
    double error = 0.0;
    for (int i = 0; i < N * N; ++i) {
        error += std::abs(result[i] - expected_result[i]);
    }
    return error;
}

double relative_error(const Matrix& result, const Matrix& expected_result, int N) {
    double error = 0.0;
    double norm_expected = 0.0;
    for (int i = 0; i < N * N; ++i) {
        norm_expected += std::abs(expected_result[i]);
    }
    error = compute_error(result, expected_result, N) / norm_expected;
    return error;
}

double computeMatrixExpression(int N, Matrix& A) {
    Matrix B, C, I;
    unsigned int seed = 42;
    initializeMatrix(B, N, seed);
    initializeMatrix(C, N, seed + 1);
    identityMatrix(I, N);

    double start_time = omp_get_wtime();
    Matrix C2(N * N, 0.0);
    Matrix C3(N * N, 0.0);
    Matrix scalarC3(N * N, 0.0);
    Matrix temp1(N * N, 0.0);
    Matrix temp2(N * N, 0.0);
    
    double traceB = 0.0;
    multiplyMatrices(C, C, C2, N);
    traceB = traceMatrix(B, N);

    multiplyMatrices(C2, C, C3, N);
    scalarMultiplyMatrix(C3, traceB, scalarC3, N);
    addMatrices(B, C2, temp1, N);
    addMatrices(temp1, scalarC3, temp2, N);
    addMatrices(temp2, I, A, N);

    double end_time = omp_get_wtime();
    std::cout << "Matrix computation completed in " << end_time - start_time << " seconds." << std::endl;
    return end_time - start_time;
}

int main() {
    std::cout << "Program started." << std::endl;
    int N = 800;
    std::cout << "N = "<< N <<std::endl;
    int max_threads = 32;
    Matrix A_1;
    std::vector<double> execution_times(max_threads + 1, 0.0);
    std::vector<double> speed_up(max_threads + 1, 0.0);
 
    omp_set_num_threads(1);
    double T1 = computeMatrixExpression(N, A_1);
    std::cout << "1-thread execution time: " << T1 << " seconds." << std::endl;
    double abs_error = compute_error(A_1, A_1, N);
    double rel_error = relative_error(A_1, A_1, N);

    execution_times[1] = T1;
    speed_up[1] = 1;

/*
    Matrix A_2;
    omp_set_num_threads(2);
    double T2 = computeMatrixExpression(N, A_2);
    std::cout << "2-thread execution time: " <<T2<< " seconds." << std::endl;
    abs_error = compute_error(A_1, A_2, N);
    rel_error = relative_error(A_1, A_2, N);
    std::cout<< ", Absolute Error (wiht T1): " << abs_error
                << ", Relative Error (wiht T1): " << rel_error 
                <<"Speed up(Sp) :"<< T1 / T2<< std::endl;

    Matrix A_4;
    omp_set_num_threads(4);
    double T4 = computeMatrixExpression(N, A_4);
    std::cout << "4-thread execution time: " <<T4<< " seconds." << std::endl;
    abs_error = compute_error(A_1, A_4, N);
    rel_error = relative_error(A_1, A_4, N);
    std::cout<< ", Absolute Error (wiht T1): " << abs_error
                << ", Relative Error (wiht T1): " << rel_error 
                <<"Speed up(Sp) :"<< T1 / T4<< std::endl;
    
    Matrix A_8;
    omp_set_num_threads(8);
    double T8 = computeMatrixExpression(N, A_8);
    std::cout << "8-thread execution time: " <<T8<< " seconds." << std::endl;
    abs_error = compute_error(A_1, A_8, N);
    rel_error = relative_error(A_1, A_8, N);
    std::cout<< ", Absolute Error (wiht T1): " << abs_error
                << ", Relative Error (wiht T1): " << rel_error 
                <<"Speed up(Sp) :"<< T1 / T8<< std::endl;

    Matrix A_16;
    omp_set_num_threads(16);
    double T16 = computeMatrixExpression(N, A_16);
    std::cout << "16-thread execution time: " <<T16<< " seconds." << std::endl;
    abs_error = compute_error(A_1, A_16, N);
    rel_error = relative_error(A_1, A_16, N);
    std::cout<< ", Absolute Error (wiht T1): " << abs_error
                << ", Relative Error (wiht T1): " << rel_error 
                <<"Speed up(Sp) :"<< T1 / T16<< std::endl;
    
    Matrix A_32;
    omp_set_num_threads(32);
    double T32 = computeMatrixExpression(N, A_32);
    std::cout << "16-thread execution time: " <<T32<< " seconds." << std::endl;
    abs_error = compute_error(A_1, A_32, N);
    rel_error = relative_error(A_1, A_32, N);
    std::cout<< ", Absolute Error (wiht T1): " << abs_error
                << ", Relative Error (wiht T1): " << rel_error 
                <<"Speed up(Sp) :"<< T1 / T32<< std::endl;
*/       
   
    for(int i = 2; i <= max_threads ; i*=2){
        Matrix A;
        omp_set_num_threads(i);
        double T = computeMatrixExpression(N, A);
        execution_times[i] = T;
        std::cout<< i << "-thread execution time: " <<T<< " seconds." << std::endl;

        double abs_error= compute_error(A_1, A, N);
        double rel_error = relative_error(A_1, A, N);
        double speedup = T1 / T;
        speed_up[i] = speedup;

        std::cout<< ", Absolute Error (wiht T1): " << abs_error
                << ", Relative Error (wiht T1): " << rel_error 
                <<", Speed up(Sp) :"<< speedup<< std::endl;
    }
    
    std::ofstream outfile("speedup_results.txt");
    if (outfile.is_open()) {
        outfile << "Threads\tExecution Time\tSpeedup\n";
        for (int i = 1; i <= max_threads; i *= 2) {
            outfile << i << "\t" << execution_times[i] << "\t" << speed_up[i] << "\n";
        }
        outfile.close();
        std::cout << "Saved to 'speedup_results.txt'." << std::endl;
    } else {
        std::cerr << "Failed to save results to file." << std::endl;
    }
    

    return 0;
}
