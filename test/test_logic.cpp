#include "catch2/catch_all.hpp"
#include "../mqtt/automation/lights.hpp"
#include "../mqtt/automation/car_heater.hpp"
#include "../mqtt/common.hpp"
#include <memory>

class TestActuator : public IActuator {
public:
    void set(std::string_view device, bool value) override {
        //this->device = device;
        //this->value = value;
    }
};

std::shared_ptr<IActuator> actuator{new TestActuator()};


TEST_CASE("Lights class test") {
    automation::Lights lights(actuator, "test_dev", hm2time(10, 0), hm2time(20, 0));
    SECTION("Turn on lights") {
        EventData item(EventId::TIME, hm2time(12, 0));
        lights.onChange(item);
        REQUIRE(lights.get() == true);
    }

    SECTION("Turn off lights") {
        EventData item(EventId::TIME, hm2time(21, 0));
        lights.onChange(item);
        REQUIRE(lights.get() == false);
    }
}


TEST_CASE("CarHeaer class test") {
    int leaveTime = hm2time(10, 0);
    automation::CarHeater heater(actuator, "tmp_dev", leaveTime);
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

        EventData item(EventId::TEMPERATURE, temperature);
        heater.onChange(item);
        EventData item2(EventId::TIME, timeAdd(leaveTime, -(offset+2)));
        heater.onChange(item2);
        INFO("Temperature: " << temperature 
             << ", Offset: " << offset 
             << ", Time: " << item2.getInt() 
             << ", on: " << heater.get());
        REQUIRE(heater.get() == false);

        EventData item3(EventId::TIME, timeAdd(leaveTime, -(offset - 2)));
        heater.onChange(item3);
        INFO("Temperature: " << temperature 
             << ", Offset: " << offset
             << ", Time: " << item3.getInt()
             << ", on: " << heater.get());
        REQUIRE(heater.get() == true);
    }
}
