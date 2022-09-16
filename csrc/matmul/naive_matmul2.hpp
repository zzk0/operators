#ifndef CSRC_MATMUL_NAIVE_MATMUL2
#define CSRC_MATMUL_NAIVE_MATMUL2

#include "benchmark/benchmark.hpp"
#include "benchmark/matmul_algorithm.hpp"

class NaiveMatmul2 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out);
};

#endif /* CSRC_MATMUL_NAIVE_MATMUL2 */
