#ifndef CSRC_MATMUL_BLIS_MATMUL
#define CSRC_MATMUL_BLIS_MATMUL

#include "benchmark/benchmark.hpp"
#include "benchmark/matmul_algorithm.hpp"

class BlisMatmul : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out);
};

#endif /* CSRC_MATMUL_BLIS_MATMUL */
