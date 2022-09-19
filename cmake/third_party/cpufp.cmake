include(ExternalProject)

set(CPUFP_ROOT_DIR ${THIRD_PARTY_DIR}/cpufp)
set(CPUFP_EXECUTABLE ${THIRD_PARTY_DIR}/cpufp/src/cpufp/cpufp)

set(CPUFP_BUILD cd ${CPUFP_ROOT_DIR}/src/cpufp && bash build.sh)
set(CPUFP_INSTALL cd ${CPUFP_ROOT_DIR}/src/cpufp && mv ${CPUFP_EXECUTABLE} ${PROJECT_BINARY_DIR})
set(CPUFP_URL https://github.com/zzk0/cpufp)
set(CPUFP_TAG master)

ExternalProject_Add(
    cpufp
    GIT_REPOSITORY ${CPUFP_URL}
    GIT_TAG ${CPUFP_TAG}
    PREFIX ${CPUFP_ROOT_DIR}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CPUFP_BUILD}
    INSTALL_COMMAND ${CPUFP_INSTALL})
