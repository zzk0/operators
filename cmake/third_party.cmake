find_package(Threads)

include(blis)

set(third_party_libs
    ${ABSL_STATIC_LIBRARIES}
    Threads::Threads)

set(third_party_dependencies
    blis)

add_custom_target(prepare_third_party ALL DEPENDS ${third_party_dependencies})
