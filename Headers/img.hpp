#ifndef IMG_HPP_
#define IMG_HPP_

#include "../Headers/Matrix.hpp"

struct Img {
  unsigned int label; 
  Matrix img_data;
  Img();
  Img(unsigned int l, Matrix data);  
  void display();
};

std::vector<Img> csv_to_imgs(const char* file_name, int number_of_images, ULL image_length, ULL image_height);

#endif // IMG_HPP_