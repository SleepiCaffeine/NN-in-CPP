#include "../Headers/Matrix.hpp"
#include <algorithm>  // std::fill()
#include <stdexcept>  // std::invalid_argument(), std::range_error(), std::length_error(), std::domain_error() <= domain expansion
#include <iostream>   // std::cout()
#include <random>     // std::random_device, std::mt19937, std::uniform_real_distribution<>
#include <iomanip>    // std::fixed(), std::setprecision()
#include <fstream>    // std::ofstream(), std::ifstream()

#define INVALID_ARR_SIZE_ERR "MATRIX DATA DECLARATION ERROR: Invalid Array specifications provided!\n"
#define COLUMN_INDEX_ERR "MATRIX DATA ACCESS ERROR: Provided collumn index exceeds the column count in matrix\n"
#define ROW_INDEX_ERR "MATRIX DATA ACCESS ERROR: Provided row index exceeds the row count in matrix\n"
#define INVALID_COLUMN_SIZE_ERR "MATRIX DIMENSION COMPARISON ERROR: Non-dentical column sizes!\n"
#define INVALID_ROW_SIZE_ERR "MATRIX DIMENSION COMPARISON ERROR: Non-Identical row sizes!\n"
#define INVALID_MATRIX_DIM_ERR "DOT ERROR: Invalid matrix dimension sizes!\n"
#define NON_FLATTENED_MATRIX_PROVIDED_ERR "LARGEST_VAL_IDX ERROR: The provided Matrix is not flattened!\n"

Matrix::Matrix() : Matrix(0, 0, 0) { };
Matrix::Matrix(ULL r, ULL c) : Matrix(r, c, 0) { }
Matrix::Matrix(ULL r, ULL c, long double val) : rows{r}, cols{c} {
  m.resize(r * c);
  fill(val);
}

Matrix::Matrix(ULL r, ULL c, long double* arr, ULL arrSize) : rows{r}, cols{c} {
  if (rows * cols != arrSize)
    throw std::length_error(INVALID_ARR_SIZE_ERR);
  this->set_as(arr, arrSize);
}

Matrix::Matrix(ULL r, ULL c, std::vector <long double> data) : rows{r}, cols{c} {
  if (rows * cols != data.size())
    throw std::length_error(INVALID_ARR_SIZE_ERR);
  m = data;
}

Matrix::Matrix(const Matrix& copy) : rows{copy.get_rows()}, cols{copy.get_cols()}, m{copy.get_matrix()} {}

void Matrix::fill(long double val) noexcept {
  std::fill(m.begin(), m.end(), val);
}

// Function that converts C-style array into Matrix
void Matrix::set_as(long double *arr, ULL arrSize) {
  // Asserting that size is valid for emplacement
  if (arrSize != this->rows * this->cols)
    throw std::length_error(INVALID_ARR_SIZE_ERR);

  // Adding all the elements into the array
  m = std::vector<long double>(arr, arr+arrSize); 
}

// Function that converts provided vector to Matrix
void Matrix::set_as(std::vector<long double> arr) {
  // Asserting that size is valid for emplacement
  if (arr.size() != rows * cols) {
    throw std::length_error(INVALID_ARR_SIZE_ERR);
  }

  // Copying over vector
  m = arr;
}

// Function that returns the value at the specific index as an R-Value
long double Matrix::at(ULL r, ULL c) const {
  if (c > cols - 1)
    throw std::range_error(COLUMN_INDEX_ERR);
  if (r > rows - 1)
    throw std::range_error(ROW_INDEX_ERR);

  return m.at(r * cols + c);
}

// Function that sets the value at the provided index
void Matrix::set_at(ULL r, ULL c, long double val) {
  if (c > cols - 1)
    throw std::range_error(COLUMN_INDEX_ERR);
  if (r > rows - 1)
    throw std::range_error(ROW_INDEX_ERR);
  m.at(r * cols + c) = val;
}

// Function to randomize the contents of the Matrix [min, max)
void Matrix::randomize(long double min, long double max) noexcept {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<long double> distrib(min, max);

  for (long double& d : m) 
    d = distrib(mt);
}

// Function that applies the function provided to the entire matrix
void Matrix::invoke_func( long double (*function)(long double) ) noexcept {
  std::transform(m.begin(), m.end(), m.begin(), function);
}

// Function to transpose the current Matrix
void Matrix::transpose() noexcept{
  // Creating a new matrix with the right dimensions 
  Matrix trans(cols, rows);
  for (ULL r = 0; r < this->rows; ++r) {
    for (ULL c = 0; c < this->cols; ++c)
      trans.m.at(c * this->rows + r) = this->at(r, c);
  }

  *this = trans;
}

void Matrix::flatten() noexcept {
  Matrix res(rows * cols, 1);
  res.set_as(m);
  *this = res;
}

// Returns vector containing Matrix data
std::vector<long double> Matrix::get_matrix() const noexcept {return m;}
// Returns the number of rows
ULL Matrix::get_rows() const noexcept {return rows;}
// Returns the number of columns
ULL Matrix::get_cols() const noexcept {return cols;}

// This fucntion expects a Matrix of dimensions n*1. If this is not provided, it will throw a std::domain_error
// Returns the index of the largest value in the Matrix
int Matrix::largest_val_idx() const {
  if (rows != 1 && cols != 1)
    throw std::domain_error(NON_FLATTENED_MATRIX_PROVIDED_ERR);
  
  long double max_val = -1;
  int idx = 0;
  for (ULL i = 0; i < m.size(); ++i) {
    long double curr_val = m.at(i);
    if (curr_val > max_val) {
      idx = i;
      max_val = curr_val;
    }
  }
  
  return idx;
}

Matrix Matrix::operator+(Matrix const& mat) const {
  check_matrix_dimensions(*this, mat);
  Matrix res(this->rows, this->cols);
  for (ULL r = 0; r <this->rows; ++r) {
    for (ULL c = 0; c < this->cols; ++c) {
      long double sum = this->at(r, c) + mat.at(r, c);
      res.set_at(r, c, sum);
    }
  }

  return res;
}

Matrix Matrix::operator+=(Matrix const& mat) const {
  return *this + mat; 
}

/* Operator overload to have Matrix Multiplication
  [NOTE: THIS FUNCTIONS AS THE 'DOT' OPERATION! FOR MULTIPLYING MATRIX VALUES GO SEE ../Headers/MatrixOperations.hpp]
*/
Matrix Matrix::operator*(Matrix const& mat) const {
  // Assert that the inner sizes are identical
  if (this->cols != mat.rows)
    throw std::domain_error(INVALID_MATRIX_DIM_ERR);
  Matrix res(this->rows, mat.cols);

  for (ULL r = 0; r < res.get_rows(); ++r) {
    for (ULL c = 0; c < res.get_cols(); ++c) {
      for (ULL n = 0; n < this->cols; ++n) {
        long double product = this->at(r, n) * mat.at(n, c);
        res.set_at(r, c, product);
      }
    }
  }

  return res;
}


Matrix Matrix::operator-(Matrix const& mat) const {
  // Make sure they're of the same size
  check_matrix_dimensions(*this, mat);
  Matrix res(this->rows, this->cols);
  for (ULL r = 0; r <this->rows; ++r) {
    for (ULL c = 0; c < this->cols; ++c) {
      long double difference = this->at(r, c) - mat.m.at(r * this->cols + c);
      res.set_at(r, c, difference);
    }
  }

  return res;
}

Matrix Matrix::operator-=(Matrix const& mat) const {
  return *this - mat; 
}

Matrix Matrix::operator*=(Matrix const& mat) const {
  return *this * mat;
}

// Helper function to display the contents of the Matrix
void Matrix::display() const noexcept {
  unsigned int col = 0;       // Saving current column number
  std::cout << "Rows: " << rows << " | Cols: " << cols << '\n';
  for (long double d : m) {   // For every matrix element
    std::cout << std::fixed << std::setprecision(4) << d << " ";      // Print it
    ++col;                      // Index current column
    if (col % cols == 0)        // If it is a multiple of 'cols'
      std::cout << "\n";          // Add a newline
  }
}

void Matrix::save(const char* file_name) const {
  std::ofstream OUTPUT(file_name);
  OUTPUT << rows << " " << cols << "\n";
  for (const long double d : m)
    OUTPUT << d << " ";
  OUTPUT.close();
}

void Matrix::save(std::string file_name) const {
  std::ofstream OUTPUT(file_name);
  OUTPUT << rows << " " << cols << "\n";
  for (const long double d : m)
    OUTPUT << d << " ";
  OUTPUT.close();
}

void Matrix::load(const char* file_name) {
  std::ifstream INPUT(file_name);
  ULL r, c;              // The reason why `r` and `c` are variables is because otherwise 
  INPUT >> r >> c;
  Matrix this_mat(r, c); // This matrix would have an unconstructed vector of size 0
  std::vector <long double> data;
  data.resize(r * c);

  for (long double& d : data)  // Meaning this wouldn't run
    INPUT >> d;
  INPUT.close();
  this_mat.set_as(data);
  *this = this_mat;
}

void Matrix::load(std::string file_name) {
  std::ifstream INPUT(file_name);
  ULL r, c;              // The reason why `r` and `c` are variables is because otherwise 
  INPUT >> r >> c;
  Matrix this_mat(r, c); // This matrix would have an unconstructed vector of size 0
  std::vector <long double> data;
  data.resize(r * c);

  for (long double& d : data)  // Meaning this wouldn't run
    INPUT >> d;
  INPUT.close();
  this_mat.set_as(data);
  *this = this_mat;
}


// Function to check if the 2 provided matrixes have identical dimensions
void check_matrix_dimensions(const Matrix m1, const Matrix m2) {
  if (m1.get_cols() != m2.get_cols())
    throw std::invalid_argument(INVALID_COLUMN_SIZE_ERR);
  if (m1.get_rows() != m2.get_rows())
    throw std::invalid_argument(INVALID_ROW_SIZE_ERR);
}