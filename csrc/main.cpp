#include <cstdlib>
#include <iostream>

#include "benchmark/benchmark.hpp"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cout << "usage: benchmark m n k\nwhere m,n is the shape of matrix a, "
                 "n,k is the shape of matrix b"
              << std::endl;
    exit(-1);
  }
  int m = atoi(argv[1]);
  int k = atoi(argv[2]);
  int n = atoi(argv[3]);

  Benchmark benchmark;
  benchmark.Launch(m, k, n);
  benchmark.Launch1(100, 1000, 100);
  return 0;
}
