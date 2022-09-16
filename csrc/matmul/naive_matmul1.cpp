#include "naive_matmul1.hpp"

void NaiveMatmul1::Matmul(int m, int k, int n, float* a, float* b, float* out) {
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

REGISTER_MATMUL(naive_matmul_temporary_variable, NaiveMatmul1);
