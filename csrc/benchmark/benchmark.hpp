#ifndef CSRC_BENCHMARK_BENCHMARK
#define CSRC_BENCHMARK_BENCHMARK

#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include "matmul_algorithm.hpp"

class Benchmark {
 public:
  Benchmark() {}
  ~Benchmark() {}

  void Launch(int m = 100, int k = 300, int n = 200);

  static void Register(const std::string& name,
                       const std::shared_ptr<MatmulAlgorithm>& algo);

  static std::unordered_map<std::string, std::shared_ptr<MatmulAlgorithm>>&
  GetMatmulAlgorithmMap();

 protected:
  void StartClock();

  int64_t StopClock();

  std::vector<float> GetRandomMatrix(int m, int n);

  bool CompareMatrices(int m, int n, float* a, float* b);

  void MatrixMatmulForValidation(
      int m, int k, int n, float* a, float* b, float* out);

 private:
  std::chrono::high_resolution_clock::time_point time_point_;
};

/**
 * TODO: fix this MACRO, it has bug: when a MatmulAlgorithm is not used,
 * registration will fail This MACRO should be used in source file otherwises,
 * multiple symbol definition would happen
 */
#define REGISTER_MATMUL(name, MatmulAlgo)                         \
  namespace {                                                     \
  struct MatmulAlgo##name {                                       \
    MatmulAlgo##name() {                                          \
      Benchmark::Register(#name, std::make_shared<MatmulAlgo>()); \
    }                                                             \
  };                                                              \
  static MatmulAlgo##name algo{};                                 \
  }

#endif /* CSRC_BENCHMARK_BENCHMARK */
