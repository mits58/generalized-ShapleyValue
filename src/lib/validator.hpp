// 愚直アルゴリズムと高速ゼータ変換アルゴリズムを二つ実行して，diff が無いかを調べる

#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP
#include "./solver.hpp"
#include <cmath>

bool eq(double a, double b) {
  constexpr double exp = 1e-9;
  return (abs(a - b) <= exp);
} 

void validate(int up) {

  for(int n = 1; n <= up; n++) {
    solver<double> sv_naive(n);
    solver<double> sv_fast(n);
    sv_naive.naive();
    sv_fast.fast_zeta_transform();

    for(int bit = 0; bit < (1 << n); bit++) {
      if(!eq(sv_naive.phi[bit], sv_fast.phi[bit])) {
        std::cout << "Error: value is different "; 
        printf("%f %f\n", sv_naive.phi[bit], sv_fast.phi[bit]); 
      }
    }
  } 
}
#endif
