#include "catch2/catch_all.hpp"
#include "../mqtt/logic.hpp"
#include "../mqtt/common.hpp"
#include <memory>

class TestActuator : public IActuator {
public:
    void set(std::string &device, bool value) override {
        //this->device = device;
        //this->value = value;
    }
};

std::shared_ptr<IActuator> actuator{new TestActuator()};


TEST_CASE("Lights class test") {
    Lights lights(actuator, hm2time(10, 0), hm2time(20, 0));
    Values state;
    SECTION("Turn on lights") {
        state.set(ValueType::TIME, hm2time(12, 0));
        lights.onChange(state);
        REQUIRE(lights.isOn() == true);
    }

    SECTION("Turn off lights") {
        state.set(ValueType::TIME, hm2time(21, 0));
        lights.onChange(state);
        REQUIRE(lights.isOn() == false);
    }
}


TEST_CASE("CarHeaer class test") {
    int leaveTime = hm2time(10, 0);
    CarHeater heater(actuator, leaveTime);
    Values state;
    auto data = GENERATE(
        std::make_pair(-21, 120),
        std::make_pair(-11,  90),
        std::make_pair( -6,  60),
        std::make_pair(  0,  30),
        std::make_pair(  4,  30)
    );
    SECTION("Temperature test") {
        float temperature = data.first;
        int offset = data.second;

        state.set(ValueType::TEMPERATURE, temperature);
        state.set(ValueType::TIME, timeAdd(leaveTime, -(offset+2)));
        heater.onChange(state);
        INFO("Temperature: " << temperature 
             << ", Offset: " << offset 
             << ", Time: " << state.getInt(ValueType::TIME) 
             << ", on: " << heater.isOn());
        REQUIRE(heater.isOn() == false);

        state.set(ValueType::TIME, timeAdd(leaveTime, -(offset - 2)));
        heater.onChange(state);
        INFO("Temperature: " << temperature 
             << ", Offset: " << offset
             << ", Time: " << state.getInt(ValueType::TIME)
             << ", on: " << heater.isOn());
        REQUIRE(heater.isOn() == true);
    }
}
