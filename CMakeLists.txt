cmake_minimum_required (VERSION 3.18)
project(gemm CXX C)

set(CMAKE_CXX_STANDARD 14 CACHE STRING "The C++ standard whose features are requested to build this target.")
set(CMAKE_C_STANDARD   11 CACHE STRING "The C standard whose features are requested to build this target.")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(THIRD_PARTY_DIR "${PROJECT_BINARY_DIR}/third_party")
set(cmake_dir ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${cmake_dir}/third_party)
list(APPEND CMAKE_MODULE_PATH ${cmake_dir})

include(third_party)
add_subdirectory(csrc)
