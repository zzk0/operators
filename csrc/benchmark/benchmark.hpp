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

  void Launch(int m, int k, int n);
  void Launch1(int start, int end, int stride);

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
 * This MACRO should be used in source file, otherwises
 * multiple symbol definition would happen
 */
#define REGISTER_MATMUL(name, MatmulAlgo)                         \
  namespace {                                                     \
  struct MatmulAlgo##name {                                       \
    MatmulAlgo##name() {                                          \
      Benchmark::Register(#name, std::make_shared<MatmulAlgo>()); \
    }                                                             \
  };                                                              \
  static MatmulAlgo##name algo##name{};                           \
  }

#endif /* CSRC_BENCHMARK_BENCHMARK */
