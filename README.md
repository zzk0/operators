# GEMM

![workflow](https://github.com/zzk0/gemm/actions/workflows/build.yml/badge.svg)

Single Floating Point Row-Major Matrix Multiplication.

# Build && Run

To build and run the project:

```bash
mkdir build
cd build
cmake ..
make -j8
./csrc/benchmark 300 400 500
```

CMake will download dependency source code from Github, configure a proxy server as needed.

```
git config --global http.proxy http://127.0.0.1:1080
git config --global https.proxy https://127.0.0.1:1080
```

# Extension

You can implement your own gemm algorithm by modifying the file `csrc/matmul/custom_matmul.cpp`.

```cpp
#include "benchmark/benchmark.hpp"
#include "benchmark/matmul_algorithm.hpp"

/**
 * Add new matrix multiply algorithm here or just modify the implementation below
 *
 * 1. define a custom class that inherits from base class MatmulAlgorithm
 */
class CustomMatmul : public MatmulAlgorithm {
 public:

  // 2. Implement your own algorithm: a is m*k, b is k*n, out = matmul(a, b)
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

// 3. Register the CustomMatmul with the name custom_matmul
REGISTER_MATMUL(custom_matmul, CustomMatmul);
```
