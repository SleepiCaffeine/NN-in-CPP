#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <vector>     // std::vector()

// To avoid writing a bunch
#define ULL unsigned long long


// Matrix class that utilizes a one-dimensional vector, only stores long doubles. Future implementations might use <T>
class Matrix {
private:
  ULL rows, cols;
  std::vector <long double> m;

public:
  // Constructors
  Matrix();
  Matrix(ULL r, ULL c);
  Matrix(ULL r, ULL c, long double val);
  Matrix(ULL r, ULL c, long double* arr, ULL arrSize);

  // Functions that affect the whole matrix

  void fill(long double val) noexcept;
  void set_as(long double* arr, ULL arrSize);
  void set_as(std::vector<long double> arr);
  void randomize(long double min, long double max) noexcept;
  void invoke_func( long double (*function)(long double) ) noexcept;
  void transpose() noexcept;
  
  // Functions to retrieve a value / values from the Matrix Object

  std::vector<long double> get_matrix() const noexcept;
  constexpr ULL get_rows() const noexcept;
  constexpr ULL get_cols() const noexcept;
  long double at(ULL r, ULL c) const;
  void set_at(ULL r, ULL c, long double val);

  // Operator overrides for the Matrix Class

  Matrix operator+(Matrix const& mat);
  Matrix operator+=(Matrix const& mat);
  Matrix operator-(Matrix const& mat);
  Matrix operator-=(Matrix const& mat);
  Matrix operator*(Matrix const& mat);

  // Helper function
  void display() const noexcept;
};

void check_matrix_dimensions(const Matrix m1, const Matrix m2);


#endif // MATRIX_HPP_