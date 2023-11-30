#ifndef NEURAL_NETWORK_HPP_
#define NEURAL_NETWORK_HPP_

#include "../Headers/img.hpp"

class NeuralNetwork {
private:
  int input, hidden, output;
  long double learning_rate;
  Matrix hidden_weights;
  Matrix output_weights;

public:
  NeuralNetwork();
  NeuralNetwork(int inp, int hid, int out, long double lr);

  void train(Matrix i_data, Matrix o_data);
  Matrix predict(Matrix input_data);
  void save(const char* directory_name);
  void load(const char* directory_name);
  void display_config() const;
};

long double sigmoidld(long double x);

#endif 