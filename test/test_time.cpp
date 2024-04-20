#include "catch2/catch_all.hpp"
#include "../mqtt/common.hpp"

TEST_CASE("addTime") {
    int t1 { hm2time(10, 30) };
    int result = timeAdd(t1, 20);
    REQUIRE(result == 1050);

    result = timeAdd(t1, -20);
    REQUIRE(result == 1010);

    result = timeAdd(t1, 60);
    REQUIRE(result == 1130);

    result = timeAdd(t1, -60);
    REQUIRE(result == 930);

    result = timeAdd(t1, -120);
    REQUIRE(result == 830);
}

TEST_CASE("addTime 2") {
    int t1 { hm2time(10, 0) };
    int result = timeAdd(t1, 60);
    REQUIRE(result == 1100);

    result = timeAdd(t1, -60);
    REQUIRE(result == 900);

    result = timeAdd(t1, -120);
    REQUIRE(result == 800);
}
