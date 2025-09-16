#include <memory>
#include <catch2/catch_all.hpp>
#include "../mqtt/automation/lights.hpp"
#include "../mqtt/automation/switch_light.hpp"
#include "../mqtt/automation/car_heater.hpp"
#include "../mqtt/common.hpp"

struct TestOutput : public IOutput {
    void send(std::string_view device, const std::string& value) override {
        lastDevice = device;
        lastValue = value;
    }

    std::string lastDevice;
    std::string lastValue;
};


TEST_CASE("Lights class test") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::Lights lights(actuator, "test_dev", hm2time(10, 0), hm2time(20, 0));
    SECTION("Turn on lights") {
        lights.onEvent(TimeEvent(12, 0));
        REQUIRE(lights.get() == true);
    }

    SECTION("Turn off lights") {
        lights.onEvent(TimeEvent(21, 0));
        REQUIRE(lights.get() == false);
    }
}

TEST_CASE("Switch mode 0") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "0");
    sw.setArg("brightness", "64");

    SECTION("Turn on - off switch") {
        sw.onButton(false);
        REQUIRE(sw.get() == true);

        sw.onButton(false);
        REQUIRE(sw.get() == false);
    }
}

TEST_CASE("Switch mode 1") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "1");
    sw.setArg("brightness", "64");

    SECTION("Turn on - off switch") {
        sw.onButton(true);
        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 64);

        sw.onButton(true);
        REQUIRE(sw.get() == false);
    }
}

TEST_CASE("Switch mode 2") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "2");

    SECTION("Turn on switch") {
        sw.setArg("brightness", "40");

        sw.onButton(true);

        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 40);
    }

    SECTION("Turn off switch") {
        sw.onButton(false);
        REQUIRE(sw.get() == false);
    }
}

TEST_CASE("Switch remembers last light value")
{
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("event", "Button Living Room");
    sw.setArg("eventLamp", "Lamp Living Room");
    sw.setArg("mode", "2");
    sw.setArg("brightness", "64");

    SECTION("Handle lamp event") {
        sw.onButton(true);
        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 64);

        sw.onLamp(12);
        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 12);

        sw.onLamp(0);
        REQUIRE(sw.get() == false);
    }
}

TEST_CASE("CarHeater class test") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    int leaveTime = hm2time(10, 0);
    automation::CarHeater heater(actuator, "tmp_dev", leaveTime);
    auto data = GENERATE(
        std::make_pair(-21, 120),
        std::make_pair(-11,  90),
        std::make_pair(-6,  60),
        std::make_pair(0,  30),
        std::make_pair(4,  30));
    SECTION("Temperature test") {
        float temperature = data.first;
        int offset = data.second;

        heater.onEvent(TemperatureEvent("Default", temperature));
        heater.onEvent(TimeEvent(10, -(offset+2)));
        bool state = heater.get();

        INFO("Temperature: " << temperature
             << ", Offset: " << offset
             << ", on: " << heater.get());
        REQUIRE(heater.get() == false);

        heater.onEvent(TimeEvent(10, -(offset - 2)));
        INFO("Temperature: " << temperature
             << ", Offset: " << offset
             << ", on: " << heater.get());
        REQUIRE(heater.get() == true);
    }
}
