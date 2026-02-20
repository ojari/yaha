#include <memory>
#include <array>
#include <nlohmann/json.hpp>

#include <gtest/gtest.h>

#include "../mqtt/automation/lights.hpp"
#include "../mqtt/automation/switch_light.hpp"
#include "../mqtt/automation/car_heater.hpp"
#include "../mqtt/common.hpp"
#include "../mqtt/automation/automation.hpp"

struct TestOutput : public automation::IAutomationOutput {
    void sendCommand(const std::string& deviceName, const automation::IAutomationOutput::CommandValues& values) override {
        lastDevice = deviceName;
        last_state = values.on;
    }

    std::string lastDevice;
    bool last_state{ false };
    int last_brightness{ 0 };
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
    auto actuator = std::make_shared<TestOutput>();
    automation::Lights lights(actuator, "test_dev");
    lights.setCondition("time", nlohmann::json::object({ {"type", "time"}, {"on", 1000}, {"off", 2000} }));

    lights.onEvent(TimeEvent(21, 0));
    EXPECT_FALSE(lights.get());

    lights.onEvent(TimeEvent(19, 59));
    EXPECT_TRUE(lights.get());
}

TEST(LightsAutomationTest, HandlesMultipleTimeRanges) {
    auto actuator = std::make_shared<TestOutput>();
    automation::Lights lights(actuator, "test_dev");

    lights.setCondition("time", nlohmann::json::object({ {"type", "time"}, {"on", 500}, {"off", 800} }));
    lights.setCondition("time", nlohmann::json::object({ {"type", "time"}, {"on", 1700}, {"off", 2000} }));

    lights.onEvent(TimeEvent(6, 30));
    EXPECT_TRUE(lights.get());

    lights.onEvent(TimeEvent(12, 0));
    EXPECT_FALSE(lights.get());

    lights.onEvent(TimeEvent(18, 0));
    EXPECT_TRUE(lights.get());
}

TEST(LightsAutomationTest, DarknessConditionOverridesSchedules) {
    auto actuator = std::make_shared<TestOutput>();
    automation::Lights lights(actuator, "test_dev");

    lights.setCondition("dark", nlohmann::json::object({ {"when", "dark"} }));

    lights.onEvent(DarkEvent(true));
    EXPECT_TRUE(lights.get());

    lights.onEvent(DarkEvent(false));
    EXPECT_FALSE(lights.get());
}

TEST(LightsAutomationTest, ElectricityPriceCanDisableLight) {
    auto actuator = std::make_shared<TestOutput>();
    automation::Lights lights(actuator, "test_dev");

    lights.setCondition("time", nlohmann::json::object({ {"on", 0},  {"off", 2300} }));
    lights.setCondition("electricity", nlohmann::json::object({ {"type", "electricity"}, {"threshold", 0.3}, {"direction", "above"}, {"requiredState", false} }));

    lights.onEvent(TimeEvent(12, 0));
    EXPECT_TRUE(lights.get());

    lights.onEvent(ElectricityPriceEvent(0.35));
    EXPECT_FALSE(lights.get());

    lights.onEvent(ElectricityPriceEvent(0.25));
    EXPECT_TRUE(lights.get());
}

TEST(SwitchModeZeroTest, TogglesOnButtonPress) {
    auto actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "0");
    sw.setArg("brightness", "64");

    sw.onButton(false);
    EXPECT_TRUE(sw.get());

    sw.onButton(false);
    EXPECT_FALSE(sw.get());
}

TEST(SwitchModeOneTest, TogglesWithBrightness) {
    auto actuator = std::make_shared<TestOutput>();
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
    auto actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "2");
    sw.setArg("brightness", "40");

    sw.onButton(true);

    EXPECT_TRUE(sw.get());
    EXPECT_EQ(sw.getInt(), 40);
}

TEST(SwitchModeTwoTest, TurnsOffOnButtonRelease) {
    auto actuator = std::make_shared<TestOutput>();
    automation::SwitchLight sw(actuator, "test_dev");
    sw.setArg("mode", "2");

    sw.onButton(false);

    EXPECT_FALSE(sw.get());
}

TEST(SwitchMemoryTest, RemembersLastLampValue) {
    auto actuator = std::make_shared<TestOutput>();
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
    auto actuator = std::make_shared<TestOutput>();
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
