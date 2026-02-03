#include <gtest/gtest.h>

#include "../mqtt/common.hpp"

TEST(TimeTest, AddTime) {
    int t1 { hm2time(10, 30) };
    int result = timeAdd(t1, 20);
    EXPECT_EQ(result, 1050);

    result = timeAdd(t1, -20);
    EXPECT_EQ(result, 1010);

    result = timeAdd(t1, 60);
    EXPECT_EQ(result, 1130);

    result = timeAdd(t1, -60);
    EXPECT_EQ(result, 930);

    result = timeAdd(t1, -120);
    EXPECT_EQ(result, 830);
}

TEST(TimeTest, AddTimeHourWrap) {
    int t1 { hm2time(10, 0) };
    int result = timeAdd(t1, 60);
    EXPECT_EQ(result, 1100);

    result = timeAdd(t1, -60);
    EXPECT_EQ(result, 900);

    result = timeAdd(t1, -120);
    EXPECT_EQ(result, 800);
}
