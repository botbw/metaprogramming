#include <gtest/gtest.h>
#include "tuple.hpp"
#include <tuple>

TEST(TupleTest, Constructor) {
    std::tuple<int, float, char> t1(1, 2.0f, 'a');
    meta::tuple<int, float, char> t2(1, 2.0f, 'a');
}