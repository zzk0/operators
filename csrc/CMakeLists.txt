project(benchmark CXX C)
include(ExternalProject)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE matmul_files ${PROJECT_SOURCE_DIR}/matmul/*.cpp)
add_library(matmul
    benchmark/benchmark.cpp
    ${matmul_files}
)

set(matmul_lib -Wl,--whole-archive matmul -Wl,--no-whole-archive)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${matmul_lib} ${third_party_libs})

add_dependencies(matmul prepare_third_party)
