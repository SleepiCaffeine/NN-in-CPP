#include "../Headers/MatrixOperations.hpp"
#include <math.h>

long double sigmoid(long double x) {
  return 1.0 / (1.0 + expl(-x));
}

Matrix add_scalar(const Matrix mat, long double scalar) {
  std::vector <long double> new_data = mat.get_matrix();
  for (long double &d : new_data)
    d += scalar;

  Matrix return_mat(mat.get_rows(), mat.get_cols());
  return_mat.set_as(new_data);
  return return_mat;
}

Matrix scale_matrix(const Matrix mat, long double scale) {
  std::vector <long double> new_data = mat.get_matrix();
  for (long double &d : new_data)
    d *= scale;

  Matrix return_mat(mat.get_rows(), mat.get_cols());
  return_mat.set_as(new_data);
  return return_mat;
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

Matrix sigmoid_prime(const Matrix mat) {
  Matrix ones(mat.get_rows(), mat.get_cols());
  ones.fill(1);
  return multiply_matrixes(mat, (ones - mat));
}

Matrix softmax(const Matrix mat) {
  long double total {0};
  for (const long double d : mat.get_matrix())
    total += expl(d);

  ULL return_mat_rows = mat.get_rows();
  ULL return_mat_cols = mat.get_cols(); 

  Matrix return_mat(return_mat_rows, return_mat_cols);

  for (ULL r = 0; r < return_mat_rows; ++r) {
    for (ULL c = 0; c < return_mat_cols; ++c) {
      long double softmaxed_val = expl(mat.at(r, c)) / total;
      return_mat.set_at(r, c, softmaxed_val);
    }
  }

  return return_mat;
}