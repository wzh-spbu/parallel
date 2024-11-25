// matrix_operations_openMP.h

#ifndef MATRIX_OPERATIONS_OPENMP_H
#define MATRIX_OPERATIONS_OPENMP_H

#include <vector>

// 定义矩阵类型
typedef std::vector<double> Matrix;

// 矩阵操作函数声明
void initializeMatrix(Matrix& mat, int N, unsigned int seed);
void identityMatrix(Matrix& mat, int N);
double traceMatrix(const Matrix& mat, int N);
void multiplyMatrices(const Matrix& A, const Matrix& B, Matrix& C, int N);
void scalarMultiplyMatrix(const Matrix& mat, double e, Matrix& result, int N);
void addMatrices(const Matrix& A, const Matrix& B, Matrix& result, int N);

#endif // MATRIX_OPERATIONS_OPENMP_H
