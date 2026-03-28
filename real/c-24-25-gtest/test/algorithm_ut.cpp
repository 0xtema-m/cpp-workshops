#include "lib/algorithm.hpp"

#include <gtest/gtest.h>

TEST(Find, SingleElement) {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto element_it = ohtuzh::find(v.begin(), v.end(), 3);
    ASSERT_EQ(
        element_it,
        v.begin() + 2
    );
}

TEST(Find, SeveralElement) {
    std::vector<int> v{1, 2, 3, 3, 4, 5};

    auto element_it = ohtuzh::find(v.begin(), v.end(), 3);
    ASSERT_EQ(
        element_it,
        v.begin() + 2
    );
}

TEST(Find, NoElement) {
    std::vector<int> v{1, 2, 3, 3, 4, 5};

    auto element_it = ohtuzh::find(v.begin(), v.end(), 6);
    ASSERT_EQ(
        element_it,
        v.end()
    );
}

struct IsEven {
    bool operator()(int value) const {
        return value % 2 == 0;
    }
};

TEST(FindIf, SingleElement) {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto element_it = ohtuzh::find_if(v.begin(), v.end(), IsEven{});
    ASSERT_EQ(
        element_it,
        v.begin() + 1
    );
}


// Same code
TEST(Accumulate, NoElements) {
    std::vector<int> v;
    const int kInit = 10;

    int result = ohtuzh::accumulate(v.begin(), v.end(), kInit);
    const int expected = kInit;

    ASSERT_EQ(result, expected);
}

TEST(Accumulate, SeveralElements) {
    std::vector<int> v{1, 2, 3};
    const int kInit = 5;

    const int result = ohtuzh::accumulate(v.begin(), v.end(), kInit);
    const int expected = kInit + 1 + 2 + 3;

    ASSERT_EQ(result, expected);
}
//

struct AccumulateIntTestParams {
    std::vector<int> Container;
    int InitialValue;
    int Expected;
};

using AccumulateTestViaTuple = std::tuple<std::vector<int>, int, int>;

class AccumulateIntTest : public ::testing::TestWithParam<AccumulateIntTestParams> {
};

TEST_P(AccumulateIntTest, AccumulateIntSum) {
    const auto& [container, initialValue, expected] = GetParam();

    const int result = ohtuzh::accumulate(
        container.begin(),
        container.end(),
        initialValue
    );
    ASSERT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    AllPossibleValues,
    AccumulateIntTest,
    ::testing::Values(
        AccumulateIntTestParams{
            .Container=std::vector<int>{}, .InitialValue=0, .Expected=0
        },
        AccumulateIntTestParams{
            .Container=std::vector<int>{}, .InitialValue=10, .Expected=10
        },
        AccumulateIntTestParams{
            .Container=std::vector<int>{1, 2, 3}, .InitialValue=5, .Expected=11
        }
    )
);
