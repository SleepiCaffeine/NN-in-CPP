#include "../Headers/NeurNetw.hpp"

void batch_img_train(NeuralNetwork& nn, std::vector<Img> images);
double test_nn(const char* test_file, const char* directory_name, unsigned int predict_size);
void train_nn(const char* train_file, const char* directory_save, unsigned int image_count);