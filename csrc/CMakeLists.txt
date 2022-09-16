project(benchmark CXX C)
include(ExternalProject)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(matmul
    benchmark/benchmark.cpp
    matmul/naive_matmul.cpp
    matmul/blis_matmul.cpp
)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} matmul ${third_party_libs})

add_dependencies(matmul prepare_third_party)
