#ifndef CSRC_BENCHMARK_MATMUL_ALGORITHM
#define CSRC_BENCHMARK_MATMUL_ALGORITHM

#include <string>

class MatmulAlgorithm {
 public:
  MatmulAlgorithm() {}
  ~MatmulAlgorithm() {}

  /**
  * a => (m, k) matrix
  * b => (k, n) matrix
  */
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) = 0;
};

#endif /* CSRC_BENCHMARK_MATMUL_ALGORITHM */
