#include "../lib/random_number.hpp"
#include "../lib/simple_function.hpp"
#include "../lib/solver.hpp"
#include "../lib/factorial.hpp"
#include <iostream>

int main() {
  
  uint32_t n; scanf("%u", &n);
  solver<double> sv(n);
  sv.naive();
  sv.fast_zeta_transform();
  return 0;
}
