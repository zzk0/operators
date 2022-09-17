#include <iostream>

#include "benchmark/benchmark.hpp"
#include "benchmark/matmul_algorithm.hpp"
#include "tools/float_comparison.hpp"

/**
 * Add new matrix multiply algorithm here or just modify the implementation below
 *
 * 1. define a custom class that inherits from base class MatmulAlgorithm
 */
class CustomMatmul : public MatmulAlgorithm {
 public:

  // 2. Implement your own algorithm: a is m*k, b is k*n, out = matmul(a, b)
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out)  {}
};

// 3. Register the CustomMatmul with the name custom_matmul
REGISTER_MATMUL(custom_matmul, CustomMatmul);
