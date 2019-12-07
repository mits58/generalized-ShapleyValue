#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP
#include <vector>

class factorial {
private:
  std::vector<uint64_t> fact;
public:
  factorial(){}
  factorial(uint32_t n) {
    fact.resize(n + 1);
    fact[0] = fact[1] = 1;
    for(size_t i = 2; i <= n; i++) {
      fact[i] = fact[i - 1] * i;
    }
  }

  uint64_t operator[](size_t k){return fact[k];};
};
#endif
