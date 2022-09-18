#include "naive_matmul.hpp"

void NaiveMatmul::Matmul(int m, int k, int n, float* a, float* b, float* out) {
  for (int x = 0; x < m; ++x) {
    for (int y = 0; y < n; ++y) {
      for (int z = 0; z < k; ++z) {
        out[x * n + y] += a[x * k + z] * b[z * n + y];
      }
    }
  }
}

REGISTER_MATMUL(naive_matmul, NaiveMatmul);

/**
 * Use temporary accumulator
 */
class NaiveMatmul1 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out)  {
    for (int x = 0; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        float sum = 0;
        for (int z = 0; z < k; ++z) {
          sum += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum;
      }
    }
  }
};

REGISTER_MATMUL(naive_matmul_temporary_variable, NaiveMatmul1);


/**
 * use multiple accumulator
 */
class NaiveMatmul2 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) {
    for (int x = 0; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        float sum = 0;
        float sum1 = 0;
        int z = 0;
        for (; z < k - 1; z += 2) {
          sum += a[x * k + z] * b[z * n + y];
          sum1 += a[x * k + (z + 1)] * b[(z + 1) * n + y];
        }
        if (z < k) {
          sum += a[x * k + z] * b[z * n + y];
        }
        sum += sum1;
        out[x * n + y] = sum;
      }
    }
  }
};

REGISTER_MATMUL(naive_matmul_multiple_accumulator, NaiveMatmul2);

/**
 * use loop unroll
 */
class NaiveMatmul3 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) {
    for (int x = 0; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        float sum = 0;
        int z = 0;
        for (; z < k - 1; z += 2) {
          sum += a[x * k + z] * b[z * n + y] + a[x * k + (z + 1)] * b[(z + 1) * n + y];
        }
        if (z < k) {
          sum += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum;
      }
    }
  }
};

REGISTER_MATMUL(naive_matmul_loop_unrool, NaiveMatmul3);

/**
 * 
 */
class NaiveMatmul4 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) {

  }
};

REGISTER_MATMUL(naive_matmul_optimized, NaiveMatmul4);

