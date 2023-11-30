#ifndef IMG_HPP_
#define IMG_HPP_

#include "../Headers/Matrix.hpp"

struct Img {
  int label; 
  Matrix img_data;
  Img();
  Img(int l, Matrix data);  
  void display();
};

std::vector<Img> csv_to_imgs(const char* file_name, unsigned int number_of_images,
                             ULL image_length, ULL image_height);

#endif // IMG_HPP_