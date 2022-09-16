#include "haswell/blis.h"

#include "blis_matmul.hpp"

void BlisMatmul::Matmul(int m, int k, int n, float* a, float* b, float* out) {
  float alpha = 1.0;
  float beta = 1.0;
  inc_t rs_a = 1;
  inc_t cs_a = m;
  inc_t rs_b = 1;
  inc_t cs_b = k;
  inc_t rs_c = 1;
  inc_t cs_c = m;
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
