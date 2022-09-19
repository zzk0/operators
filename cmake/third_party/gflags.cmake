include(ExternalProject)

set(GFLAGS_ROOT_DIR ${THIRD_PARTY_DIR}/gflags)
set(GFLAGS_INCLUDE_DIR ${GFLAGS_ROOT_DIR}/src/gflags-build/include)
set(GFLAGS_LIBRARY_DIR ${GFLAGS_ROOT_DIR}/src/gflags-build/lib)
set(GFLAGS_URL https://github.com/gflags/gflags/archive/refs/tags/v2.2.2.zip)

include_directories(${GFLAGS_INCLUDE_DIR})
set(GFLAGS_LIBRARY_NAMES
    libgflags.a)
foreach(LIBRARY_NAME ${GFLAGS_LIBRARY_NAMES})
    list(APPEND GFLAGS_STATIC_LIBRARIES ${GFLAGS_LIBRARY_DIR}/${LIBRARY_NAME})
endforeach()

ExternalProject_Add(
    gflags
    URL ${GFLAGS_URL}
    PREFIX ${GFLAGS_ROOT_DIR}
    BUILD_BYPRODUCTS ${GFLAGS_STATIC_LIBRARIES}
    INSTALL_COMMAND ""
    TEST_COMMAND "")
