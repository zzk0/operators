#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>

#include "haswell/blis.h"

#include "benchmark.hpp"
#include "matmul/blis_matmul.hpp"
#include "matmul/naive_matmul.hpp"
#include "tools/progress_bar.hpp"
#include "tools/float_comparison.hpp"

namespace {

void PrintMatrix(float* mat, int m, int n) {
  std::cout << std::endl;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << mat[i * n + j] << " ";
    }
    std::cout << std::endl;
  }
}

}  // namespace

void Benchmark::Launch(int m, int k, int n) {
  constexpr int experiment_times = 20;
  constexpr int NANO_PER_MILLI = 1000000;

  std::vector<std::tuple<float, float, float, std::string>> algo_time_cost;

  int algo_id = 0;
  PrintProgress(0.0);
  for (const auto& it : GetMatmulAlgorithmMap()) {
    const std::string& name = it.first;
    const std::shared_ptr<MatmulAlgorithm>& algo = it.second;

    int64_t total = 0, min_cost = INT64_MAX, max_cost = INT64_MIN;
    for (int i = 0; i < experiment_times; ++i) {
      std::vector<float> a = GetRandomMatrix(m, k);
      std::vector<float> b = GetRandomMatrix(k, n);
      std::vector<float> c(m * n, 0.0f);
      std::vector<float> ans(m * n, 0.0f);
      MatrixMatmulForValidation(m, k, n, a.data(), b.data(), ans.data());

      // TODO: add timeout
      StartClock();
      algo->Matmul(m, k, n, a.data(), b.data(), c.data());
      int64_t duration = StopClock();

      if (!CompareMatrices(m, n, c.data(), ans.data())) {
        std::cout << name << std::endl;
        PrintMatrix(c.data(), m, n);
        PrintMatrix(ans.data(), m, n);
        std::cerr << "Wrong Answer" << std::endl;
        exit(-1);
      }
      total += duration;
      min_cost = std::min(min_cost, duration);
      max_cost = std::max(max_cost, duration);

      PrintProgress(static_cast<double>(algo_id) /
                        GetMatmulAlgorithmMap().size() +
                    1.0 / GetMatmulAlgorithmMap().size() *
                    static_cast<double>(i + 1) / experiment_times);
    }

    algo_time_cost.emplace_back(
        static_cast<float>(total) / NANO_PER_MILLI / experiment_times,
        static_cast<float>(min_cost) / NANO_PER_MILLI,
        static_cast<float>(max_cost) / NANO_PER_MILLI,
        name);
    algo_id += 1;
  }

  std::sort(algo_time_cost.begin(), algo_time_cost.end());
  std::cout << "\n\nalgorithm\t\taverage\t\tmin\t\tmax\n";
  for (const auto& t : algo_time_cost) {
    std::cout << std::get<3>(t) << "\t\t" << std::get<0>(t) << "\t\t"
              << std::get<1>(t) << "\t\t" << std::get<2>(t) << std::endl;
  }
}

void Benchmark::Register(const std::string& name,
                         const std::shared_ptr<MatmulAlgorithm>& algo) {
  auto it = GetMatmulAlgorithmMap().find(name);
  if (it != GetMatmulAlgorithmMap().end()) {
    std::cerr << "Matmul Algorigthm has been registered" << std::endl;
    exit(-1);
  }
  GetMatmulAlgorithmMap()[name] = algo;
}

std::unordered_map<std::string, std::shared_ptr<MatmulAlgorithm>>&
Benchmark::GetMatmulAlgorithmMap() {
  static std::unordered_map<std::string, std::shared_ptr<MatmulAlgorithm>>
      algos;
  return algos;
}

void Benchmark::StartClock() { time_point_ = std::chrono::system_clock::now(); }

int64_t Benchmark::StopClock() {
  auto duration = std::chrono::system_clock::now() - time_point_;
  return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

std::vector<float> Benchmark::GetRandomMatrix(int m, int n) {
  std::vector<float> mat(m * n, 0.0f);
  std::random_device device;
  std::mt19937 engine(device());
  std::uniform_real_distribution<> dist(-1.0f, 1.0f);
  for (int i = 0; i < m * n; ++i) {
    mat[i] = dist(engine);
  }
  return mat;
}

bool Benchmark::CompareMatrices(int m, int n, float* a, float* b) {
  for (int i = 0; i < m * n; ++i) {
    if (!FloatEqual(a[i], b[i], 0.01)) {
      std::cout << a[i] << " " << b[i] << std::endl;
      return false;
    }
  }
  return true;
}

void Benchmark::MatrixMatmulForValidation(
    int m, int k, int n, float* a, float* b, float* out) {
  NaiveMatmul algo;
  BlisMatmul algo1;
  algo1.Matmul(m, k, n, a, b, out);
}
