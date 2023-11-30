// Small Header file to generate random numbers

#ifndef RNG_H_
#define RNG_H_

#include <random>

namespace RNG {

template <typename T>
T gen_rand(T min, T max) noexcept {
  std::random_device rd;    // Generates a random seed for the RNG
  std::mt19937 mt(rd());    // standard mersenne_twister_engine seeded with rd() {https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution}
  std::uniform_real_distribution<T> distribution(min, max);   // Distribution of T type elements [min, max)
  return distribution(mt);   // Calls distribution to convert random unsigned int to T
}

};

#endif