#include "catch2/catch_all.hpp"
#include "../mqtt/logic.hpp"
#include "../mqtt/common.hpp"

TEST_CASE("Lights class test") {
    Lights lights(hm2time(10, 0), hm2time(20, 0));
    Values state;
    SECTION("Turn on lights") {
        state.setTime(hm2time(12, 0));
        lights.onChange(state);
        REQUIRE(lights.isOn() == true);
    }

    SECTION("Turn off lights") {
        state.setTime(hm2time(21, 0));
        lights.onChange(state);
        REQUIRE(lights.isOn() == false);
    }
}


TEST_CASE("CarHeaer class test") {
    CarHeater heater(hm2time(10, 0));
    Values state;
    SECTION("Temperature -21") {
        state.setTemperature(-21);
        state.setTime(hm2time(7, 59));
        heater.onChange(state);
        REQUIRE(heater.isOn() == false);

        state.setTime(hm2time(8, 1));
        heater.onChange(state);
        REQUIRE(heater.isOn() == true);
    }
}
