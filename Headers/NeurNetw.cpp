#include "../Headers/NeurNetw.hpp"
#include "../Headers/MatrixOperations.hpp"
#include <math.h>
#include <fstream>
#include <iostream>
// For Directory creation
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>

#define OUTPUT_MATRIX_DATA_SAVE_FILE_LOCATION "NN_OUTPUT_WEIGHTS.txt"
#define HIDDEN_MATRIX_DATA_SAVE_FILE_LOCATION "NN_HIDDEN_WEIGHTS.txt"
#define NEURAL_NETWORK_CONFIG_SAVE_FILE_LOCATION "NN_CONFIG.txt"

void diss_matrixes(const Matrix m1, const Matrix m2) {
  std::cout << "Multiplying Matrix of dimensions: [" << m1.get_rows() << "; " << m1.get_cols() << "] with Matrix of dimensions: [" <<
            m2.get_rows() << "; " << m2.get_cols() << "]\n";
}

long double sigmoidld(long double x) {
  return 1.0 / (1.0 + expl(-x));
}

NeuralNetwork::NeuralNetwork(int inp, int hid, int out, long double lr) : 
  input{inp}, hidden{hid}, output{out}, learning_rate{lr},
  hidden_weights{Matrix(hidden, input)}, output_weights{Matrix(output, hidden)} {

    hidden_weights.randomize(0, 1);
    output_weights.randomize(0, 1);
}

NeuralNetwork::NeuralNetwork() : NeuralNetwork(0, 0, 0, 0) { }

void NeuralNetwork::train(Matrix i_data, Matrix o_data) {

  // Feed forward the given input matrix
  Matrix hidden_i = this->hidden_weights * i_data;
  Matrix hidden_o = hidden_i;
  hidden_o.invoke_func(sigmoidld);
  Matrix final_i  = this->output_weights * hidden_o;
  Matrix final_o  = final_i;
  final_o.invoke_func(sigmoidld);

  // Find the Difference Between Expected and Calculated Outputs
  Matrix output_err = o_data - final_o;
  Matrix t_output   = this->output_weights;
  t_output.transpose();
  Matrix hidden_err = t_output * output_err;

  // Backpropogation algorithm [OUTPUT]
  Matrix sig_final           = sigmoid_prime(final_o);
  Matrix multiplied_o_err    = multiply_matrixes(output_err, sig_final);
  Matrix transposed_hidden_o = hidden_o;
  transposed_hidden_o.transpose();

  Matrix scaled_err = multiplied_o_err * transposed_hidden_o;
  scaled_err = scale_matrix(scaled_err, this->learning_rate);

  this->output_weights += scaled_err;

  // Backpropogation algorithm [HIDDEN]
  Matrix sig_hidden       = sigmoid_prime(hidden_o);
  Matrix multiplied_h_err = multiply_matrixes(hidden_err, sig_hidden);
  Matrix transposed_i     = i_data;
  transposed_i.transpose();
  scaled_err = multiplied_h_err * transposed_i;
  scaled_err = scale_matrix(scaled_err, this->learning_rate);
  this->hidden_weights += scaled_err;
}

Matrix NeuralNetwork::predict(const Matrix i_data) {
  Matrix hidden_o = this->hidden_weights * i_data;
  std::cout << "HIDDEN LAYER OUTPUT WEIGHTS:\n";
  hidden_o.invoke_func(sigmoidld);
  hidden_o.display();
  Matrix final_o  = this->output_weights * hidden_o;
  final_o.invoke_func(sigmoidld);
  std::cout << "\nAND NOW FINAL OUTPUTS (PRE-SM):\n";
  final_o.display();
  return softmax(final_o);
}

void NeuralNetwork::save(const char* directory_name) {
  mkdir(directory_name, 0777);

  std::string new_file_location = directory_name;
  new_file_location += "/";

  std::ofstream OUTPUT(new_file_location + NEURAL_NETWORK_CONFIG_SAVE_FILE_LOCATION);
  OUTPUT << this->input << "\n" << this->hidden << "\n" << this->output << "\n" << this->learning_rate << "\n";
  OUTPUT.close();
  this->hidden_weights.save(new_file_location + HIDDEN_MATRIX_DATA_SAVE_FILE_LOCATION);
  this->output_weights.save(new_file_location + OUTPUT_MATRIX_DATA_SAVE_FILE_LOCATION);
}

void NeuralNetwork::load(const char* directory_name) {
  std::string new_file_location = directory_name;
  new_file_location += "/";
  std::ifstream INPUT(new_file_location + NEURAL_NETWORK_CONFIG_SAVE_FILE_LOCATION);
  INPUT >> input >> hidden >> output >> learning_rate;
  INPUT.close();
  this->hidden_weights.load(new_file_location + HIDDEN_MATRIX_DATA_SAVE_FILE_LOCATION);
  this->output_weights.load(new_file_location + OUTPUT_MATRIX_DATA_SAVE_FILE_LOCATION);
}

void NeuralNetwork::display_config() const {
  std::cout << "Displaying Neural Network internal configuration:\n";
  std::cout << "INPUTS: " << input;
  std::cout << "\nHIDDEN LAYERS: " << hidden;
  std::cout << "\nOUTPUTS: " << output;
  std::cout << "\nLEARNING_RATE: " << learning_rate << "\n";
}
