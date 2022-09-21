#include <immintrin.h>
#include <pmmintrin.h>
#include <xmmintrin.h>
#include <x86intrin.h>

#include "naive_matmul.hpp"

/**
  * 访存量分析：
  * a, b, out 都是 m * n * k 次，因为就在三重循环里面
  * 计算量分析：
  * m * n * k * 2，三层循环里面共有一次加法，一次乘法
 */
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
 * CSAPP: Use temporary accumulator
 * out 的访存量可以减少
 * 总的访存量：a b 分别是 m * n * k，而 out 是 m * n
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
 * CSAPP: use multiple accumulator
 * 访存量保持不变，可以利用更多寄存器
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
 * 访存量保持不变，可以更好的并行
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
 * 行优先存储的情况下，L1 64 byte，也就是 16 个 float
 * out 的 cache miss 次数为：m * n / 16
 * a 的 cache miss 次数为：m * k / 16 * n
 * b 的 cache miss 次数为：k * n * m
 * 为了减少 b 的 cache miss，可以尽可能一次计算，复用数组 b
 * 每次用 4 行和 b 的一列进行乘法
 * b 的 cache miss 次数变为 k * n * m / 4
 */
class NaiveMatmul4 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) {
    int x = 0;
    for (; x < m; x += 4) {
      for (int y = 0; y < n; ++y) {
        float sum0 = 0;
        float sum1 = 0;
        float sum2 = 0;
        float sum3 = 0;
        for (int z = 0; z < k; ++z) {
          sum0 += a[x * k + z] * b[z * n + y];
          sum1 += a[(x + 1) * k + z] * b[z * n + y];
          sum2 += a[(x + 2) * k + z] * b[z * n + y];
          sum3 += a[(x + 3) * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum0;
        out[(x + 1) * n + y] = sum1;
        out[(x + 2) * n + y] = sum2;
        out[(x + 3) * n + y] = sum3;
      }
    }
    for (; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        float sum0 = 0;
        for (int z = 0; z < k; ++z) {
          sum0 += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum0;
      }
    }
  }
};

REGISTER_MATMUL(naive_matmul_4x1, NaiveMatmul4);

/**
 * 以上优化尽可能复用矩阵 b，以下优化尽可能复用矩阵 a
 * 上面访问  a[(x + 0...3) * k + z] 来尽可能复用 b
 * 每次读取 b[z * n + y] 的时候，会读取多 16 个 float
 * 可以尝试复用读取 b 时候，利用多读取的这些 float
 *
 * 最终的优化效果并不是很明显，在实验次数较少（20）的情况下，偶尔会比不过 4x1
 * 通过增加次数（200）可以稍微增加一些胜出次数。
 *
 * 分析：
 * 这次的优化减少了 a 的 cache miss，减少为 m * k / 16 * n / 4
 */
class NaiveMatmul5 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) {
    int x = 0;
    for (; x < m; x += 4) {
      int y = 0;
      for (; y < n; y += 4) {
        float sum00 = 0, sum01 = 0, sum02 = 0, sum03 = 0;
        float sum10 = 0, sum11 = 0, sum12 = 0, sum13 = 0;
        float sum20 = 0, sum21 = 0, sum22 = 0, sum23 = 0;
        float sum30 = 0, sum31 = 0, sum32 = 0, sum33 = 0;
        for (int z = 0; z < k; ++z) {
          sum00 += a[x * k + z] * b[z * n + y];
          sum10 += a[x * k + z] * b[z * n + y + 1];
          sum20 += a[x * k + z] * b[z * n + y + 2];
          sum30 += a[x * k + z] * b[z * n + y + 3];
          sum01 += a[(x + 1) * k + z] * b[z * n + y];
          sum02 += a[(x + 2) * k + z] * b[z * n + y];
          sum03 += a[(x + 3) * k + z] * b[z * n + y];

          sum11 += a[(x + 1) * k + z] * b[z * n + y + 1];
          sum12 += a[(x + 2) * k + z] * b[z * n + y + 1];
          sum13 += a[(x + 3) * k + z] * b[z * n + y + 1];

          sum21 += a[(x + 1) * k + z] * b[z * n + y + 2];
          sum22 += a[(x + 2) * k + z] * b[z * n + y + 2];
          sum23 += a[(x + 3) * k + z] * b[z * n + y + 2];

          sum31 += a[(x + 1) * k + z] * b[z * n + y + 3];
          sum32 += a[(x + 2) * k + z] * b[z * n + y + 3];
          sum33 += a[(x + 3) * k + z] * b[z * n + y + 3];
        }
        out[x * n + y] = sum00;
        out[(x + 1) * n + y] = sum01;
        out[(x + 2) * n + y] = sum02;
        out[(x + 3) * n + y] = sum03;

        out[x * n + y + 1] = sum10;
        out[(x + 1) * n + y + 1] = sum11;
        out[(x + 2) * n + y + 1] = sum12;
        out[(x + 3) * n + y + 1] = sum13;

        out[x * n + y + 2] = sum20;
        out[(x + 1) * n + y + 2] = sum21;
        out[(x + 2) * n + y + 2] = sum22;
        out[(x + 3) * n + y + 2] = sum23;

        out[x * n + y + 3] = sum30;
        out[(x + 1) * n + y + 3] = sum31;
        out[(x + 2) * n + y + 3] = sum32;
        out[(x + 3) * n + y + 3] = sum33;
      }
      for (; y < n; ++y) {
        int sum = 0;
        for (int z = 0; z < k; ++z) {
          sum += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum;
      }
    }
    for (; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        float sum0 = 0;
        for (int z = 0; z < k; ++z) {
          sum0 += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum0;
      }
    }
  }
};

REGISTER_MATMUL(naive_matmul_4x4, NaiveMatmul5);

/**
 * 将寄存器都用上，这个技巧似乎没有奏效。可能编译器会帮我做这个事情。
 */
class NaiveMatmul6 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) {
    int x = 0;
    for (; x < m; x += 4) {
      int y = 0;
      for (; y < n; y += 4) {
        register float sum00 = 0, sum01 = 0, sum02 = 0, sum03 = 0;
        register float sum10 = 0, sum11 = 0, sum12 = 0, sum13 = 0;
        register float sum20 = 0, sum21 = 0, sum22 = 0, sum23 = 0;
        register float sum30 = 0, sum31 = 0, sum32 = 0, sum33 = 0;
        register float b0 = 0, b1 = 0, b2 = 0, b3 = 0;
        for (int z = 0; z < k; ++z) {
          b0 = b[z * n + y];
          b1 = b[z * n + y + 1];
          b2 = b[z * n + y + 2];
          b3 = b[z * n + y + 3];

          sum00 += a[x * k + z] * b0;
          sum01 += a[(x + 1) * k + z] * b0;
          sum02 += a[(x + 2) * k + z] * b0;
          sum03 += a[(x + 3) * k + z] * b0;

          sum10 += a[x * k + z] * b1;
          sum11 += a[(x + 1) * k + z] * b1;
          sum12 += a[(x + 2) * k + z] * b1;
          sum13 += a[(x + 3) * k + z] * b1;

          sum20 += a[x * k + z] * b2;
          sum21 += a[(x + 1) * k + z] * b2;
          sum22 += a[(x + 2) * k + z] * b2;
          sum23 += a[(x + 3) * k + z] * b2;

          sum30 += a[x * k + z] * b3;
          sum31 += a[(x + 1) * k + z] * b3;
          sum32 += a[(x + 2) * k + z] * b3;
          sum33 += a[(x + 3) * k + z] * b3;
        }
        out[x * n + y] = sum00;
        out[(x + 1) * n + y] = sum01;
        out[(x + 2) * n + y] = sum02;
        out[(x + 3) * n + y] = sum03;

        out[x * n + y + 1] = sum10;
        out[(x + 1) * n + y + 1] = sum11;
        out[(x + 2) * n + y + 1] = sum12;
        out[(x + 3) * n + y + 1] = sum13;

        out[x * n + y + 2] = sum20;
        out[(x + 1) * n + y + 2] = sum21;
        out[(x + 2) * n + y + 2] = sum22;
        out[(x + 3) * n + y + 2] = sum23;

        out[x * n + y + 3] = sum30;
        out[(x + 1) * n + y + 3] = sum31;
        out[(x + 2) * n + y + 3] = sum32;
        out[(x + 3) * n + y + 3] = sum33;
      }
      for (; y < n; ++y) {
        int sum = 0;
        for (int z = 0; z < k; ++z) {
          sum += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum;
      }
    }
    for (; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        float sum0 = 0;
        for (int z = 0; z < k; ++z) {
          sum0 += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum0;
      }
    }
  }
};

// REGISTER_MATMUL(naive_matmul_4x4_register, NaiveMatmul6);


/**
 * 使用 SSE 指令提高并发程度
 */
class NaiveMatmul7 : public MatmulAlgorithm {
 public:
  virtual void Matmul(int m, int k, int n, float* a, float* b, float* out) {
    int x = 0;
    for (; x < m - 3; x += 4) {
      int y = 0;
      for (; y < n - 3; y += 4) {
        __m128 sum0 = _mm_set1_ps(0), sum1 = _mm_set1_ps(0),
               sum2 = _mm_set1_ps(0), sum3 = _mm_set1_ps(0);
        for (int z = 0; z < k; ++z) {
          __m128 a0_04 = _mm_load1_ps(&a[x * k + z]);
          __m128 a1_04 = _mm_load1_ps(&a[(x + 1) * k + z]);
          __m128 a2_04 = _mm_load1_ps(&a[(x + 2) * k + z]);
          __m128 a3_04 = _mm_load1_ps(&a[(x + 3) * k + z]);
          __m128 b04 = _mm_load_ps(&b[z * n + y]);

          sum0 = _mm_add_ps(sum0, _mm_mul_ps(a0_04, b04));
          sum1 = _mm_add_ps(sum1, _mm_mul_ps(a1_04, b04));
          sum2 = _mm_add_ps(sum2, _mm_mul_ps(a2_04, b04));
          sum3 = _mm_add_ps(sum3, _mm_mul_ps(a3_04, b04));
        }
        _mm_store_ps(&out[x * n + y], sum0);
        _mm_store_ps(&out[(x + 1) * n + y], sum1);
        _mm_store_ps(&out[(x + 2) * n + y], sum2);
        _mm_store_ps(&out[(x + 3) * n + y], sum3);
      }
      for (; y < n; ++y) {
        int sum = 0;
        for (int z = 0; z < k; ++z) {
          sum += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum;
      }
    }
    for (; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        float sum0 = 0;
        for (int z = 0; z < k; ++z) {
          sum0 += a[x * k + z] * b[z * n + y];
        }
        out[x * n + y] = sum0;
      }
    }
  }
};

REGISTER_MATMUL(naive_matmul_4x4_SSE, NaiveMatmul7);
