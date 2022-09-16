#include "naive_matmul2.hpp"

void NaiveMatmul2::Matmul(int m, int k, int n, float* a, float* b, float* out) {
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

REGISTER_MATMUL(naive_matmul_loop_unroll, NaiveMatmul2);
