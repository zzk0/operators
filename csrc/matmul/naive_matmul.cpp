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
