#include <iostream>
#include "../Headers/img.hpp"
#include "../Headers/testing_nn.hpp"

void batch_img_train(NeuralNetwork& nn, std::vector<Img> images) {
  for (ULL img = 0; img < images.size(); ++img) {
    if (!(img % 100))
      std::cout << img << " images have been processed...\n";

    Img curr_img = images.at(img);
    Matrix image_data = curr_img.img_data;
    image_data.flatten();

    Matrix expected_output(10, 1);                  // Create matrix to store output
    expected_output.set_at(curr_img.label, 0, 1);   // Set Image label as expected output
    nn.train(image_data, expected_output);         // Train NN on image data said expected output
  }
}

double test_nn(const char* test_file, const char* directory_name, unsigned int predict_size) {
  NeuralNetwork nn;
  nn.load(directory_name);
  nn.display_config();
  auto images = csv_to_imgs(test_file, predict_size, 0, 0);
  int correct_guess = 0;

  for (unsigned int i = 0; i < predict_size; ++i) {
    Img image = images.at(i);
    Matrix image_data = image.img_data;
    image_data.flatten();
    Matrix prediction = nn.predict(image_data);

    std::cout << "Image " << i+1 << " | Expected: " << image.label << " | Actual: " << prediction.largest_val_idx() << "\n";
    if (prediction.largest_val_idx() == image.label)
      ++correct_guess;
  }
  std::cout << "it guessed correctly " << correct_guess << " times\n";
  return 1.0 * correct_guess / predict_size;
}

void train_nn(const char* train_file, const char* directory_save, unsigned int image_count) {
  auto images = csv_to_imgs(train_file, image_count, 0, 0);
  NeuralNetwork nn(784, 100, 10, 0.01);
  batch_img_train(nn, images);
  nn.save(directory_save);
}
