#include "lib/serialization.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(Data, Serialize) {
    ohtuzh::data data("some_name", {1, 2, 3}, ohtuzh::state::kOk);
    const std::string expected = "some_name|1,2,3,|OK\n";

    std::stringstream ss;
    data.serialize(ss);

    ASSERT_EQ(ss.str(), expected);
}

TEST(Data, Deserialize) {
    ohtuzh::data expected("some_name", {1, 2, 3}, ohtuzh::state::kOk);
    std::stringstream ss("some_name|1,2,3,|OK\n");

    ohtuzh::data real;
    real.deserialize(ss);

    ASSERT_EQ(real, expected);
}

// Iteration 0: Simple Test
TEST(Data, SerializeAndDeserialize) {
    ohtuzh::data to_serialize("some_name", {1, 2, 3}, ohtuzh::state::kOk);

    std::stringstream ss;
    to_serialize.serialize(ss);

    ohtuzh::data to_deserialize;
    to_deserialize.deserialize(ss);

    ASSERT_EQ(to_serialize, to_deserialize);
}


// Iteration 1: Simple Parametrized Test

// using SerializeAndDeserializeTestData = ohtuzh::data;
struct SerializeAndDeserializeTestData {
    ohtuzh::data Data;
};

class SimpleParametrizedSerializeAndDeserializeTestSuite
        : public ::testing::TestWithParam<SerializeAndDeserializeTestData> {};

TEST_P(
    SimpleParametrizedSerializeAndDeserializeTestSuite,
    SerializeAndDeserializeBack
) {
    const auto& [to_serialize] = GetParam();

    std::stringstream ss;
    to_serialize.serialize(ss);

    ohtuzh::data to_deserialize;
    to_deserialize.deserialize(ss);

    ASSERT_EQ(to_serialize, to_deserialize);
}

INSTANTIATE_TEST_SUITE_P(
    AllPossibleData,
    SimpleParametrizedSerializeAndDeserializeTestSuite,
    ::testing::Values(
        SerializeAndDeserializeTestData{
            .Data = ohtuzh::data("", {1, 2, 3}, ohtuzh::state::kOk)
        },
        SerializeAndDeserializeTestData{
            .Data = ohtuzh::data("name", {}, ohtuzh::state::kOk)
        },
        SerializeAndDeserializeTestData{
            .Data = ohtuzh::data("name", {1, 2, 3}, ohtuzh::state::kOk)
        }
    )
);

// Iteration 2: Cartesian Product

class MyTestSuite
    : public ::testing::TestWithParam<
        std::tuple<std::string, std::vector<int32_t>, ohtuzh::state>
    > {};

TEST_P(MyTestSuite, Combine) {
    const auto& [name, numbers, state] = GetParam();
    ohtuzh::data test_data(name, numbers, state);

    std::stringstream ss;
    test_data.serialize(ss);

    ohtuzh::data deserialized_data;
    deserialized_data.deserialize(ss);

    ASSERT_EQ(test_data, deserialized_data);
}

INSTANTIATE_TEST_SUITE_P(
    CombinedGroup,
    MyTestSuite,
    testing::Combine(
        testing::Values("", "abc", "abcdefghqwe"),
        testing::Values(std::vector<int32_t>{}, std::vector<int32_t>{1, 2, 3}, std::vector<int32_t>{1, 2, 3, 4, 5, 6, 7, 8}),
        testing::Values(ohtuzh::state::kBad, ohtuzh::state::kOk)
    )
);
