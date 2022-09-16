#include "haswell/blis.h"

#include "blis_matmul.hpp"

void BlisMatmul::Matmul(int m, int k, int n, float* a, float* b, float* out) {
  float alpha = 1.0f;
  float beta = 0.0f;
  inc_t rs_a = k;
  inc_t cs_a = 1;
  inc_t rs_b = n;
  inc_t cs_b = 1;
  inc_t rs_c = n;
  inc_t cs_c = 1;

  // row-major (rs > 1, cs = 1), column-major (rs = 1, cs > 1)
  bli_sgemm(BLIS_NO_TRANSPOSE,
            BLIS_NO_TRANSPOSE,
            m,
            n,
            k,
            &alpha,
            a,
            rs_a,
            cs_a,
            b,
            rs_b,
            cs_b,
            &beta,
            out,
            rs_c,
            cs_c);
}

REGISTER_MATMUL(blis_matmul, BlisMatmul);
