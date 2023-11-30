#ifndef MATRIX_OPERATIONS_
#define MATRIX_OPERATIONS_
#include "../Headers/Matrix.hpp"

long double sigmoid(long double x);

Matrix add_scalar(const Matrix mat, long double scalar);
Matrix scale_matrix(const Matrix mat, long double scale);
Matrix multiply_matrixes(const Matrix m1, const Matrix m2);
Matrix sigmoid_prime(const Matrix mat);
Matrix softmax(const Matrix mat);

#endif // MATRIX_OPERATIONS_