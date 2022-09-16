#ifndef CSRC_MATMUL_NAIVE_MATMUL1
#define CSRC_MATMUL_NAIVE_MATMUL1

#include "benchmark/benchmark.hpp"
#include "benchmark/matmul_algorithm.hpp"

class NaiveMatmul1 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out);
};


#endif /* CSRC_MATMUL_NAIVE_MATMUL1 */
