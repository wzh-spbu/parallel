// sequential_execution_openMP.cpp
#include <map>
#include <iostream>
#include <vector>
#include <omp.h>
#include <fstream>
#include "matrix_operations_openMP.h"
#include <time.h>
#include <cmath>
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
    int max_threads = 32;
    Matrix A_1;
    std::map<int, std::vector<double>> execution_times; // 每个 N 对应的线程运行时间
    std::map<int, std::vector<double>> speed_ups;       // 每个 N 对应的线程加速比
    std::map<int, std::vector<double>> absolute_errors;  // 每个 N 对应的线程绝对误差
    std::map<int, std::vector<double>> relative_errors;  // 每个 N 对应的线程相对误差

    for(int N = 800;N <= 1500;N = N + 100){
        std::cout << "N = "<< N <<std::endl;
        std::vector<double> times;
        std::vector<double> speedups;
        std::vector<double> abs_errors;    // 保存绝对误差
        std::vector<double> rel_errors;    // 保存相对误差

        omp_set_num_threads(1);
        double T1 = computeMatrixExpression(N, A_1);
        std::cout << "1-thread execution time: " << T1 << " seconds." << std::endl;
        times.push_back(T1);
        speedups.push_back(1);
        abs_errors.push_back(0); 
        rel_errors.push_back(0);

        for(int i = 2; i <= max_threads ; i*=2){
            Matrix A;
            omp_set_num_threads(i);
            double T = computeMatrixExpression(N, A);
            times.push_back(T);

            std::cout<< i << "-thread execution time: " <<T<< " seconds." << std::endl;
            double abs_error = compute_error(A_1, A, N); // 多线程绝对误差
            double rel_error = relative_error(A_1, A, N); // 多线程相对误差
            abs_errors.push_back(abs_error);
            rel_errors.push_back(rel_error);

            double speedup = T1 / T;
            speedups.push_back(speedup);

            std::cout<< ", Absolute Error (wiht 1-thread): " << abs_error
                    << ", Relative Error (wiht 1-thread): " << rel_error 
                    <<", Speed up(Sp) :"<< speedup<< std::endl;   
        }
        execution_times[N] = times;
        speed_ups[N] = speedups;
        absolute_errors[N] = abs_errors;
        relative_errors[N] = rel_errors;
    }   

    
    
    
    std::ofstream outfile("speedup_results.txt");
    if (outfile.is_open()) {
    outfile << "N\tThreads\tExecution Time\tSpeedup\tAbsolute Error\tRelative Error\n";
    for (const auto& [N, times] : execution_times) {
        const auto& speedups = speed_ups.at(N);
        const auto& abs_errors = absolute_errors.at(N);
        const auto& rel_errors = relative_errors.at(N);

        size_t num_threads = times.size();
        for (size_t i = 0; i < num_threads; ++i) {
            outfile << N << "\t"                
                    << std::pow(2, i) << "\t"          
                    << times[i] << "\t"
                    << speedups[i] << "\t"
                    << abs_errors[i] << "\t"
                    << rel_errors[i] << "\n";
        }
    }

        outfile.close();
        std::cout << "Results successfully saved to 'speedup_results.txt'." << std::endl;
    } else {
        std::cerr << "Failed to save results to file." << std::endl;
    }
    

    return 0;
}
