#include <memory>
#include <catch2/catch_all.hpp>
#include "../mqtt/automation/lights.hpp"
#include "../mqtt/automation/switch_light.hpp"
#include "../mqtt/automation/car_heater.hpp"
#include "../mqtt/common.hpp"
#include "../mqtt/event_data.hpp"

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

TEST_CASE("Switch mode 0") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("event", "Button Living Room");
    sw.setArg("mode", "0");
    sw.setArg("brightness", "64");

    SECTION("Turn on - off switch") {
        EventData item(EventId::BUTTON_LIVING_ROOM, 0);
        sw.onChange(item);
        REQUIRE(sw.get() == true);

        sw.onChange(item);
        REQUIRE(sw.get() == false);
    }
}

TEST_CASE("Switch mode 1") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("event", "Button Living Room");
    sw.setArg("mode", "1");
    sw.setArg("brightness", "64");

    SECTION("Turn on - off switch") {
        EventData item(EventId::BUTTON_LIVING_ROOM, 1);
        sw.onChange(item);
        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 64);

        sw.onChange(item);
        REQUIRE(sw.get() == false);
    }
}

TEST_CASE("Switch mode 2") {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("event", "Button Living Room");
    sw.setArg("mode", "2");

    SECTION("Turn on switch") {
        sw.setArg("brightness", "40");

        EventData item(EventId::BUTTON_LIVING_ROOM, 1);
        sw.onChange(item);

        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 40);
    }

    SECTION("Turn off switch") {
        EventData item(EventId::BUTTON_LIVING_ROOM, 0);
        sw.onChange(item);
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
        EventData ev1(EventId::BUTTON_LIVING_ROOM, 1);
        sw.onChange(ev1);
        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 64);

        EventData ev2(EventId::LAMP_LIVING_ROOM, 12);
        sw.onChange(ev2);
        REQUIRE(sw.get() == true);
        REQUIRE(sw.getInt() == 12);

        ev2.set(0);
        sw.onChange(ev2);
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
