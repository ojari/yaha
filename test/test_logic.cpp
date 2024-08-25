#include "catch2/catch_all.hpp"
#include "../mqtt/controller/lights.hpp"
#include "../mqtt/controller/car_heater.hpp"
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
    controller::Lights lights(actuator, hm2time(10, 0), hm2time(20, 0));
    SECTION("Turn on lights") {
        ValueItem item(ValueType::TIME, hm2time(12, 0));
        lights.onChange(item);
        REQUIRE(lights.isOn() == true);
    }

    SECTION("Turn off lights") {
        ValueItem item(ValueType::TIME, hm2time(21, 0));
        lights.onChange(item);
        REQUIRE(lights.isOn() == false);
    }
}


TEST_CASE("CarHeaer class test") {
    int leaveTime = hm2time(10, 0);
    controller::CarHeater heater(actuator, leaveTime);
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

        ValueItem item(ValueType::TEMPERATURE, temperature);
        heater.onChange(item);
        ValueItem item2(ValueType::TIME, timeAdd(leaveTime, -(offset+2)));
        heater.onChange(item2);
        INFO("Temperature: " << temperature 
             << ", Offset: " << offset 
             << ", Time: " << item2.getInt() 
             << ", on: " << heater.isOn());
        REQUIRE(heater.isOn() == false);

        ValueItem item3(ValueType::TIME, timeAdd(leaveTime, -(offset - 2)));
        heater.onChange(item3);
        INFO("Temperature: " << temperature 
             << ", Offset: " << offset
             << ", Time: " << item3.getInt()
             << ", on: " << heater.isOn());
        REQUIRE(heater.isOn() == true);
    }
}
