#include "Matrix.hpp"
#include <algorithm>  // std::fill()
#include <stdexcept>  // std::invalid_argument(), std::range_error(), std::length_error(), std::domain_error() <= domain expansion
#include <iostream>   // std::cout()
#include <random>     // std::random_device, std::mt19937, std::uniform_real_distribution<>
#include <iomanip>    // std::fixed(), std::setprecision()

// Error Messages. Since error messages don't need type checking, I'm defining these
#define INVALID_ARR_SIZE_ERR "Invalid Array specifications provided!\n"
#define COLUMN_INDEX_ERR "Provided collumn index exceeds the column count in matrix\n"
#define ROW_INDEX_ERR "Provided row index exceeds the row count in matrix\n"
#define INVALID_COLUMN_SIZE_ERR "Non-dentical column sizes!\n"
#define INVALID_ROW_SIZE_ERR "Non-Identical row sizes!\n"
#define INVALID_MATRIX_DIM_ERR "Invalid matrix dimension sizes!\n"

// Default Constructor
Matrix::Matrix() : Matrix(0, 0) { };

// Custom Constructor That creates a matrix of size r x c, and fills it with 0
Matrix::Matrix(ULL r, ULL c) : Matrix(r, c, 0) { }

// Custom Constructor That creates a matrix of size r x c, and fills it with val
Matrix::Matrix(ULL r, ULL c, long double val) : rows{r}, cols{c} {
  m.resize(r * c);
  fill(val);
}

// Custom Constructor That creates a matrix of size r x c, and fills it with the elements of arr
Matrix::Matrix(ULL r, ULL c, long double* arr, ULL arrSize) : rows{r}, cols{c} {
  *this = Matrix(r, c);
  this->set_as(arr, arrSize);
}

// Function to fill Matrix with given value
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

// Function that returns the value at the specific index. This is only an rvalue!!!
long double Matrix::at(ULL r, ULL c) const {
  if (c > cols - 1)
    throw std::range_error(COLUMN_INDEX_ERR);
  if (r > rows - 1)
    throw std::range_error(ROW_INDEX_ERR);

  return get_matrix().at(r * cols + c);
}

// Function that sets the value at the provided index
void Matrix::set_at(ULL r, ULL c, long double val) {
  if (c > this->cols - 1)
    throw std::range_error(COLUMN_INDEX_ERR);
  if (r > this->rows - 1)
    throw std::range_error(ROW_INDEX_ERR);
  this->m.at(r * this->cols + c) = val;
}

// Function that converts provided vector to Matrix
void Matrix::set_as(std::vector<long double> arr) {
  // Asserting that size is valid for emplacement
  if (arr.size() != m.size())
    throw std::length_error(INVALID_ARR_SIZE_ERR);

  // Copying over vector
  m = arr;
}

// Function to randomize the contents of the Matrix [min, max)
void Matrix::randomize(long double min, long double max) noexcept {
  std::random_device rd;        // Generates a random seed for the RNG
  std::mt19937 mt(rd);          // standard mersenne_twister_engine seeded with rd() {https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution}
  std::uniform_real_distribution<long double> distrib(min, max);    // Distribution of T type elements [min, max)

  for (long double& d : m) 
    d = distrib(mt);            // Calls distribution to convert random unsigned int to long double
}

// Function that applies the function provided to the entire matrix
void Matrix::invoke_func( long double (*function)(long double) ) noexcept {
  std::transform(m.begin(), m.end(), m.begin(), function);
}

// Function to transpose the current Matrix
void Matrix::transpose() noexcept{
  // Creating a new matrix with the right dimensions 
  Matrix* trans = new Matrix(this->cols, this->rows);

  for (ULL r = 0; r < this->rows; ++r) {
    for (ULL c = 0; c < this->cols; ++c) {
      // I will admit, I have no idea how I came up with this
      // I just messed around with it and it works!
      trans->m.at(c * this->rows + r) = this->at(r, c);
    }  
  }

  *this = *trans;   // Copying pointer data to this Matrix
  delete trans;     // This sounds bad, but also funny
}

// Returns vector that contains Matrix data
std::vector<long double> Matrix::get_matrix() const noexcept {return m;}
// Returns the number of columns
constexpr ULL Matrix::get_rows() const noexcept {return rows;}
// Returns the number of rows
constexpr ULL Matrix::get_cols() const noexcept {return cols;}

// Operator overload to add Matrixes
Matrix Matrix::operator+(Matrix const& mat) {
  // Make sure they're of the same size
  check_matrix_dimensions(*this, mat);

  Matrix res(this->rows, this->cols);       // Create a Matrix to return
  for (ULL r = 0; r <this->rows; ++r) {     // For Every Row in Both Matrixes
    for (ULL c = 0; c < this->cols; ++c) {  // For Every Collumn in Both Matrixes 
      long double sum = (this->at(r, c) + mat.at(r, c));
      res.set_at(r, c, sum);  // Add them both to the result Matrix
    }
  }

  // Return the resulting matrix
  return res;
}

// Operator overload to += Matrixes
Matrix Matrix::operator+=(Matrix const& mat) {
  return *this + mat; 
}

// Operator overload to have Matrix Multiplication [NOTE: THIS FUNCTIONS AS THE 'DOT' OPERATION!]
Matrix Matrix::operator*(Matrix const& mat) {
  // Assert that the inner sizes are identical
  if (this->cols != mat.rows)
    throw std::domain_error(INVALID_MATRIX_DIM_ERR);
  
  // Create a Matrix to return
  Matrix res(this->rows, mat.cols);

  for (ULL r = 0; r < res.get_rows(); ++r) {      // For every row
    for (ULL c = 0; c < res.get_cols(); ++c) {    // For every collumn
      for (ULL n = 0; n < this->cols; ++n) {      // For each element in the inner matrix dimensions
        long double product = this->at(r, n) * mat.at(n, c);
        res.set_at(r, c, product);
      }
    }
  }
  // Return the resulting Matrix
  return res;
}

// Operator overload to - Matrixes
Matrix Matrix::operator-(Matrix const& mat) {
  // Make sure they're of the same size

  Matrix res(this->rows, this->cols);       // Create a Matrix to return
  for (ULL r = 0; r <this->rows; ++r) {     // For Every Row in Both Matrixes
    for (ULL c = 0; c < this->cols; ++c) {  // For Every Collumn in Both Matrixes  
      long double difference = this->at(r, c) - mat.m.at(r * this->cols + c);     // Get the difference
      res.set_at(r, c, difference);                                               // And add it up
    }
  }
  // Return the resulting matrix
  return res;
}

// Operator overload to -= Matrixes
Matrix Matrix::operator-=(Matrix const& mat) {
  return *this - mat; 
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


// Function to check if the 2 provided matrixes have identical dimensions
void check_matrix_dimensions(const Matrix m1, const Matrix m2) {
  if (m1.get_cols() != m2.get_cols())
    throw std::invalid_argument(INVALID_COLUMN_SIZE_ERR);
  if (m1.get_rows() != m2.get_rows())
    throw std::invalid_argument(INVALID_ROW_SIZE_ERR);
}