#include <memory>
#include <array>

#include <gtest/gtest.h>

#include "../mqtt/automation/lights.hpp"
#include "../mqtt/automation/switch_light.hpp"
#include "../mqtt/automation/car_heater.hpp"
#include "../mqtt/common.hpp"

struct TestOutput : public IOutput {
    void send(std::string_view device, const std::string& value) override {
        //lastDevice = device;
        //lastValue = value;
    }

    //std::string lastDevice;
    //std::string lastValue;
};


/* TEST(LightsAutomationTest, TurnsOnWithinWindow) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::Lights lights(actuator, "test_dev", hm2time(10, 0), hm2time(20, 0));

    lights.onEvent(TimeEvent(12, 0));

    EXPECT_TRUE(lights.get());
}
*/

TEST(LightsAutomationTest, TurnsOffOutsideWindow) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::Lights lights(actuator, "test_dev", hm2time(10, 0), hm2time(20, 0));

    lights.onEvent(TimeEvent(21, 0));

    EXPECT_FALSE(lights.get());
}

TEST(SwitchModeZeroTest, TogglesOnButtonPress) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "0");
    sw.setArg("brightness", "64");

    sw.onButton(false);
    EXPECT_TRUE(sw.get());

    sw.onButton(false);
    EXPECT_FALSE(sw.get());
}

TEST(SwitchModeOneTest, TogglesWithBrightness) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "1");
    sw.setArg("brightness", "64");

    sw.onButton(true);
    EXPECT_TRUE(sw.get());
    EXPECT_EQ(sw.getInt(), 64);

    sw.onButton(true);
    EXPECT_FALSE(sw.get());
}

TEST(SwitchModeTwoTest, TurnsOnWithCustomBrightness) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "2");
    sw.setArg("brightness", "40");

    sw.onButton(true);

    EXPECT_TRUE(sw.get());
    EXPECT_EQ(sw.getInt(), 40);
}

TEST(SwitchModeTwoTest, TurnsOffOnButtonRelease) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "2");

    sw.onButton(false);

    EXPECT_FALSE(sw.get());
}

TEST(SwitchMemoryTest, RemembersLastLampValue) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("event", "Button Living Room");
    sw.setArg("eventLamp", "Lamp Living Room");
    sw.setArg("mode", "2");
    sw.setArg("brightness", "64");

    sw.onButton(true);
    EXPECT_TRUE(sw.get());
    EXPECT_EQ(sw.getInt(), 64);

    sw.onLamp(12);
    EXPECT_TRUE(sw.get());
    EXPECT_EQ(sw.getInt(), 12);

    sw.onLamp(0);
    EXPECT_FALSE(sw.get());
}

TEST(CarHeaterTest, ActivatesBasedOnTemperatureOffsets) {
    std::shared_ptr<IOutput> actuator = std::make_shared<TestOutput>();
    const int leaveTime = hm2time(10, 0);
    automation::CarHeater heater(actuator, "tmp_dev", leaveTime);

    const std::array<std::pair<int, int>, 5> data {{
        { -21, 120 },
        { -11,  90 },
        {  -6,  60 },
        {   0,  30 },
        {   4,  30 }
    }};

    for (const auto& entry : data) {
        const float temperature = static_cast<float>(entry.first);
        const int offset = entry.second;
        //SCOPED_TRACE(::testing::Message()
        //    << "Temperature: " << temperature
        //    << " offset: " << offset);

        heater.onEvent(TemperatureEvent("Default", temperature));
        heater.onEvent(TimeEvent(10, -(offset + 2)));
        EXPECT_FALSE(heater.get());

        heater.onEvent(TimeEvent(10, -(offset - 2)));
        EXPECT_TRUE(heater.get());
    }
}
