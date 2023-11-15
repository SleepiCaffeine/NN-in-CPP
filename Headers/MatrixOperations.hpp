#ifndef MATRIX_OPERATIONS_
#define MATRIX_OPERATIONS_
#include "../Headers/Matrix.hpp"

void add_scalar(Matrix& mat, long double scalar) {
  for (long double& d : mat.get_matrix())
    d += scalar;
}

void scale_matrix(Matrix& mat, long double scale) {
  for (long double& d : mat.get_matrix())
    d *= scale;
}

Matrix multiply_matrixes(const Matrix m1, const Matrix m2) {
  check_matrix_dimensions(m1, m2);
  Matrix result(m2.get_rows(), m1.get_cols());

  for (ULL r = 0; r < m1.get_rows(); ++r) {
    for (ULL c = 0; c < m2.get_cols(); ++c) {
      long double product = m1.at(r, c) * m2.at(r, c);
      result.set_at(r, c, product);
    }
  }

  return result;
}

#endif // MATRIX_OPERATIONS_