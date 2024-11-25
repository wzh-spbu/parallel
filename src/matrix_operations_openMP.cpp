// matrix_operations_openMP.cpp

#include "matrix_operations_openMP.h"
#include <omp.h>
#include <cstdlib>
#include <iostream>

void initializeMatrix(Matrix& mat, int N, unsigned int seed) {
    mat.resize(N * N);
    std::srand(seed);
    for (int i = 0; i < N * N; ++i) {
        mat[i] = static_cast<double>(std::rand()) / RAND_MAX;
    }
}

void identityMatrix(Matrix& mat, int N) {
    mat.resize(N * N, 0.0);
    for (int i = 0; i < N; ++i) {
        mat[i * N + i] = 1.0;
    }
}

double traceMatrix(const Matrix& mat, int N) {
    double trace = 0.0;
    #pragma omp parallel for reduction(+:trace)
    for (int i = 0; i < N; ++i) {
        trace += mat[i * N + i];
    }
    return trace;
}

void multiplyMatrices(const Matrix& A, const Matrix& B, Matrix& C, int N) {
    C.resize(N * N, 0.0);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

void scalarMultiplyMatrix(const Matrix& mat, double e, Matrix& result, int N) {
    result.resize(N * N);
    #pragma omp parallel for
    for (int i = 0; i < N * N; ++i) {
        result[i] = mat[i] * e;
    }
}

void addMatrices(const Matrix& A, const Matrix& B, Matrix& result, int N) {
    result.resize(N * N);
    #pragma omp parallel for
    for (int i = 0; i < N * N; ++i) {
        result[i] = A[i] + B[i];
    }
}
