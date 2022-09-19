find_package(Threads)

include(blis)
include(tabulate)
include(cpufp)
include(gflags)

set(third_party_libs
    ${BLIS_STATIC_LIBRARIES}
    ${GFLAGS_STATIC_LIBRARIES}
    Threads::Threads)

set(third_party_dependencies
    blis
    tabulate
    gflags)

add_custom_target(prepare_third_party ALL DEPENDS ${third_party_dependencies})
