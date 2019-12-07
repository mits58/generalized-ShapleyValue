#ifndef SOLVER
#define SOLVER
#include <iostream>
#include <vector>
#include <bitset>
#include <unistd.h>
#include "./simple_function.hpp"
#include "./factorial.hpp"

template<typename T> // f: 2^P -> T
class solver {
public:
  uint32_t n;
  uint64_t P; // {1, 2, ..., n}
  std::vector<T> phi;
  std::vector<T> sub1;
  std::vector<T> sub2;
  factorial fact;

  solver(){};
  solver(uint32_t n):n(n) {
    phi.resize(1 << n);
    fact = factorial(n);
    P = (1 << n) - 1;
  };

  void naive() {
    std::cout << "naive" << std::endl;
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      int64_t M = P & ~delta;           // M = P / delta
      T sum = 0;
      for(int S = M; S >= 0; S = (S - 1) & M){
        // std::cerr << std::bitset<4>(S) << std::endl;
        uint32_t size_S = __builtin_popcount(S);
        uint32_t size_delta = __builtin_popcount(delta);
        T add = fact[size_S];
        add *= fact[size_delta];
        add *= fact[n - size_S - size_delta];
        add /= fact[n];
        T mul = simple_function(S | delta) - simple_function(S);
        add *= mul;
        sum += add;
        if(S == 0) break;
      }
      std::cerr << "phi[ " << delta << " ] = " << sum << std::endl;
      phi[delta] = sum;
    }
  };

  void fast_zeta_transform() {
    std::cout << "fast" << std::endl;
    sub1.resize(1 << n);
    sub2.resize(1 << n);
    
    culc_sub1();
    culc_sub1_naive();
    culc_sub2();
    culc_sub2_naive();

    // phi[delta] = sub1[delta] - sub2[delta]; 線型性より
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      phi[delta] = sub1[delta] - sub2[delta];
      std::cerr << "phi[ " << delta << " ] = " << phi[delta] << std::endl;
    }
  };

  void culc_sub1() {
    auto g = [&](uint64_t bit) {
      uint32_t size_bit = __builtin_popcount(bit);
      T ret = simple_function(bit);
      ret *= fact[n - size_bit];
      ret /= fact[n];
      return ret;
    };
    
    std::vector<std::vector<std::vector<T>>> dp(n + 1, std::vector<std::vector<T>> (n + 1, std::vector<T> (1 << n, 0)));
    
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      dp[0][0][delta] = g(delta);
    }

    // fast zeta transform
    for(uint32_t i = 1; i <= n; i++) {
      for(uint64_t delta = 0; delta < (1 << n); delta++) {
        for(uint32_t sz = 0; sz <= n; sz++) {
          dp[i][sz][delta] += dp[i - 1][sz][delta];

          if(!((1 << (i - 1)) & delta)) {
            if(sz != 0) dp[i][sz][delta] += dp[i - 1][sz - 1][delta | (1 << (i - 1))] * sz;
          }
        }
      }
    }
    
    // sz に関して集計
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      T sum = 0.0;
      for(uint32_t sz = 0; sz <= n; sz++) {
        sum += dp[n][sz][delta];
      }
      sub1[delta] = sum * fact[__builtin_popcount(delta)];
    }

    // debug
    /*
    std::cerr << "output sub1 with fast algorithm" << std::endl;
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      std::cerr << "sub1[" << delta << "] = " << sub1[delta] << std::endl;
    }*/
  };

  void culc_sub1_naive() {
    
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      int64_t M = P & ~delta;           // M = P / delta
      T sum = 0;
      for(int S = M; S >= 0; S = (S - 1) & M){
        // std::cerr << std::bitset<4>(S) << std::endl;
        uint32_t size_S = __builtin_popcount(S);
        uint32_t size_delta = __builtin_popcount(delta);
        T add = fact[size_S];
        add *= fact[size_delta];
        add *= fact[n - size_S - size_delta];
        add /= fact[n];
        T mul = simple_function(S | delta);
        add *= mul;
        sum += add;
        if(S == 0) break;
      }
      sub1[delta] = sum;
    }

    // debug
    /*
    std::cerr << "output sub1 with naive algorithm" << std::endl;
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      std::cerr << "sub1[" << delta << "] = " << sub1[delta] << std::endl;
    }
    */
  };

  void culc_sub2() {
    auto g = [&](uint64_t bit) {
      uint32_t size_bit = __builtin_popcount(bit);
      T ret = simple_function(bit);
      ret *= fact[size_bit];
      ret /= fact[n];
      return ret;
    };
    
    std::vector<std::vector<std::vector<T>>> dp(n + 1, std::vector<std::vector<T>> (n + 1, std::vector<T> (1 << n, 0.0)));
    
    for(uint64_t U = 0; U < (1 << n); U++) {
      dp[0][0][U] = g(U);
    }

    // fast zeta transform
    for(uint32_t i = 1; i <= n; i++) {
      for(uint64_t U = 0; U < (1 << n); U++) {
        for(uint32_t sz = 0; sz <= n; sz++) {
          dp[i][sz][U] += dp[i - 1][sz][U];
          
          // if (i - 1) in U
          if(U >> (i - 1) & 1) {
            if(sz != 0) dp[i][sz][U] += dp[i - 1][sz - 1][U & ~(1 << (i - 1))] * sz;
          }
        }
      }
    }
    
    // sz に関して集計
    for(uint64_t U = 0; U < (1 << n); U++) {
      T sum = 0.0;
      uint64_t delta = P & ~U;
      for(uint32_t sz = 0; sz <= n; sz++) {
        sum += dp[n][sz][U];
      }
      assert(n - __builtin_popcount(U) == __builtin_popcount(delta));
      sub2[delta] = sum * fact[n - __builtin_popcount(U)];
    }

    // debug
    /*
    std::cerr << "output sub2 with fast algorithm" << std::endl;
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      std::cerr << "sub2[" << delta << "] = " << sub2[delta] << std::endl;
    }
    */
  };

  void culc_sub2_naive() {
    
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      int64_t M = P & ~delta;           // M = P / delta
      T sum = 0;
      for(int S = M; S >= 0; S = (S - 1) & M){
        // std::cerr << std::bitset<4>(S) << std::endl;
        uint32_t size_S = __builtin_popcount(S);
        uint32_t size_delta = __builtin_popcount(delta);
        T add = fact[size_S];
        add *= fact[size_delta];
        add *= fact[n - size_S - size_delta];
        add /= fact[n];
        T mul = simple_function(S);
        add *= mul;
        sum += add;
        if(S == 0) break;
      }
      sub2[delta] = sum;
    }

    // debug
    /*
    std::cerr << "output sub2 with naive algorithm" << std::endl;
    for(uint64_t delta = 0; delta < (1 << n); delta++) {
      std::cerr << "sub2[" << delta << "] = " << sub2[delta] << std::endl;
    }
    */
  };

};

#endif
