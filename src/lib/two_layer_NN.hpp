#ifndef two_LAYER_NN
#define two_LAYER_NN
#include "./random_number.hpp"
#include "./one_layer_NN.hpp"

uint64_t seed = 8473829184842ull;

double two_layer_nn(uint64_t S, uint32_t n, uint32_t k) {
  double ret = 0.0;
  random_number rnd(seed);
  for(uint32_t i = 0; i < k; i++) {
    double z = one_layer_nn(S, n, rnd);
    // ReLU
    if(z < 0.0) z = 0.0;
    ret += z;
  }
  return ret;
}

#endif
