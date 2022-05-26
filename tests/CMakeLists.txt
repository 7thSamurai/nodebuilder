include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/master.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    nodebuilder_test
    vec_test.cpp
)

target_link_libraries(
    nodebuilder_test
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(nodebuilder_test)
