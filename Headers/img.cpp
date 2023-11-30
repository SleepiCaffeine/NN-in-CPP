#include "../Headers/img.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <math.h>

#define IMAGE_SIZE 28

Img::Img() : Img(0, {}) { };
Img::Img(int l , Matrix data) : label{l}, img_data{data} { };


std::vector<Img> csv_to_imgs(const char* file_name, unsigned int number_of_images, ULL image_length = 0, ULL image_height = 0) {
  std::ifstream INPUT(file_name);      // Opening up .csv file
  if (!INPUT.is_open()) {
    throw std::invalid_argument("File failed to open. Maybe invalid path specified\n");
  }

  std::vector<Img> images;             // Creating the return vector of `Img`s
  images.resize(number_of_images);     // Reserving memory in `Img` vector to contain images

  std::string row;                    // String to hold the entire row of a csv file (an Image)
  std::getline(INPUT, row);           // Reading the first row (not useful)

  std::string token;                      // String to hold each individual string
  std::vector<long double> image_pixels;  // Vector to store tokens turned into ld for Matrixes

  // For every image && while the file isn't empty
  for (unsigned int i = 0; !INPUT.eof() && i < number_of_images; ++i) {
    Img curr_img;

    // Intitializing / Clearing values
    token = "";
    image_pixels.clear();

    // Read the entire line and tokenize it 
    std::getline(INPUT, row, '\n');
    std::istringstream iss(row);      // Create String stream to further tokenize

    while (std::getline(iss, token, ',')) {
      image_pixels.push_back(std::stold(token) / 256.0);
    }

    // First value in each image is label, so we store it as separate value, and remove it
    long double img_label = image_pixels.at(0) * 256.0;
    image_pixels.erase(image_pixels.begin());
    curr_img.label = img_label;

    // If image sizes are not provided, we will assume the image is a square
    if (!image_height && !image_length) {
      image_height = std::sqrt(image_pixels.size());
      image_length = image_height;
    }

    // Create adequately sides matrix
    Matrix mat(image_height, image_length, 0);
    // Sets matrix data as the image pixel values
    mat.set_as(image_pixels);
    // Set Image data as matrix
    curr_img.img_data = mat;
    // Add current image to the list of images
    images.at(i) = curr_img;
  }

  std::cout << "Finished reading all " << number_of_images << " images.\n";
  INPUT.close();    // Close File
  return images;    // Return images
}

void Img::display() {
  std::cout << "Displaying Image of Label: " << label << "\n";
  img_data.display();
}