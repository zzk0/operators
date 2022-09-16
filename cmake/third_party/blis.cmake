# Many thanks to https://github.com/Oneflow-Inc/oneflow
#   and https://blog.cachecha.com/csdn/xueqiu1141/84074618.html

include(ExternalProject)

set(BLIS_ROOT_DIR ${THIRD_PARTY_DIR}/blis)
set(BLIS_INCLUDE_DIR ${THIRD_PARTY_DIR}/blis/src/blis/include)
set(BLIS_LIBRARY_DIR ${THIRD_PARTY_DIR}/blis/src/blis/lib)

set(BLIS_URL https://github.com/flame/blis/archive/refs/tags/0.9.0.zip)
set(BLIS_CONFIGURE cd ${BLIS_ROOT_DIR}/src/blis && ./configure auto)
set(BLIS_BUILD cd ${BLIS_ROOT_DIR}/src/blis && $(MAKE))
set(BLIS_TEST cd ${BLIS_ROOT_DIR}/src/blis && $(MAKE) check)

include_directories(${BLIS_INCLUDE_DIR})
set(BLIS_LIBRARY_NAMES
    haswell/libblis.a)
foreach(LIBRARY_NAME ${BLIS_LIBRARY_NAMES})
    list(APPEND BLIS_STATIC_LIBRARIES ${BLIS_LIBRARY_DIR}/${LIBRARY_NAME})
endforeach()

ExternalProject_Add(
    blis
    URL ${BLIS_URL}
    PREFIX ${BLIS_ROOT_DIR}
    BUILD_BYPRODUCTS ${BLIS_STATIC_LIBRARIES}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ${BLIS_CONFIGURE}
    BUILD_COMMAND ${BLIS_BUILD}
    INSTALL_COMMAND ""
    TEST_COMMAND ${BLIS_TEST})

