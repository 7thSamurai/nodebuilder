include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/master.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    nodebuilder_test
    common_test.cpp
    vec_test.cpp
    line_test.cpp
    box_test.cpp
    color_test.cpp
)

target_link_libraries(
    nodebuilder_test
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(nodebuilder_test)
