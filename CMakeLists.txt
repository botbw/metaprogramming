cmake_minimum_required(VERSION 3.5.0)

project(metaprogramming VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

################## google test ##################

# from gtest: https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    test_tuple
    tests/test_tuple.cpp
)
target_link_libraries(
    test_tuple
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_tuple)