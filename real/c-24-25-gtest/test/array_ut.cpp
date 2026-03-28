#include "lib/array.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(Array, Size) {
    ohtuzh::array<int, 5> arr;
    ASSERT_EQ(arr.size(), 5) << "Some comment";
}

TEST(Array, Init) {
    ohtuzh::array<int, 5> arr{1, 2, 3, 4, 5};
    ASSERT_THAT(arr, ::testing::ElementsAre(1, 2, 3, 4, 5));
}

// TODO: tests for .at() with exceptions
