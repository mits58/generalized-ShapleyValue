#ifndef ONE_LAYER_NN
#define ONE_LAYER_NN
#include "./random_number.hpp"
#include "../utils/constraints.hpp"

double one_layer_nn(uint64_t S, uint32_t n, random_number &rnd) {
  double ret = 0.0;
  for(uint32_t i = 0; i < n; i++) {
    double w = rnd.next_double(MIN_W, MAX_W);
    if(S >> i & 1) ret += w;
  }
  return ret;
}

#endif
