#include "../lib/solver.hpp"
#include "../lib/validator.hpp"
#include <iostream>
#include <string>

int main(int argv, char **argc) {
  
  if(argv < 3) {
    std::cerr << "Usage: " << argc[0] << " validate|naive|fast <size_of_P>" << std::endl;
    return 1;
  }

  std::string mode = argc[1];
  uint32_t n = atoi(argc[2]);
  if(mode == "validate") {
    validate(n);
  } else if (mode == "naive") {
    solver<double> sv(n);
    sv.naive();
  } else {
    solver<double> sv(n);
    sv.fast_zeta_transform();
  }
  return 0;
}
