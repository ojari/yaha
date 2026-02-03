#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "../mqtt/device/light_device.hpp"
#include "mocks.h"

namespace device {

struct MockListener {
    explicit MockListener(EventBus& evbus) {
        evbus.subscribe<LampEvent>([&](const LampEvent& e) {
            location = e.location;
            brightness = e.brightness;
            notified = true;
        });
    }

    std::string location;
    int brightness = -1;
    bool notified = false;
};

TEST(LightDeviceTest, ConstructionSendsOnCommand) {
    EventBus evbus;
    LightDevice device("bedroom_light", evbus);
    MockOutput output;

    device.send(output, true, 100);

    EXPECT_EQ(output.lastTopic, "zigbee2mqtt/bedroom_light/set");
    EXPECT_NE(output.lastPayload.find("\"state\": \"ON\""), std::string::npos);
    EXPECT_NE(output.lastPayload.find("\"brightness\": 100"), std::string::npos);
}

TEST(LightDeviceOnMessageTest, ProcessesOnStateWithBrightness) {
    const std::string deviceName = "bedroom_light";
    EventBus evbus;
    MockListener mock(evbus);
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", "ON"},
        {"brightness", 150}
    };

    device.onMessage(deviceName, payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_EQ(mock.brightness, 150);
}

TEST(LightDeviceOnMessageTest, ProcessesOffState) {
    const std::string deviceName = "bedroom_light";
    EventBus evbus;
    MockListener mock(evbus);
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", "OFF"},
        {"brightness", 150}
    };

    device.onMessage(deviceName, payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_EQ(mock.brightness, 0);
}

TEST(LightDeviceOnMessageTest, ProcessesFalseState) {
    const std::string deviceName = "bedroom_light";
    EventBus evbus;
    MockListener mock(evbus);
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", false},
        {"brightness", 200}
    };

    device.onMessage(deviceName, payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_EQ(mock.brightness, 0);
}

TEST(LightDeviceSendTest, SendsOnCommandVariants) {
    EventBus evbus;
    LightDevice device("kitchen_light", evbus);
    MockOutput output;

    device.send(output, true, 100);
    EXPECT_EQ(output.lastTopic, "zigbee2mqtt/kitchen_light/set");
    EXPECT_EQ(output.lastPayload, R"({"state": "ON", "brightness": 100})");

    device.send(output, true, 255);
    EXPECT_EQ(output.lastPayload, R"({"state": "ON", "brightness": 255})");

    device.send(output, true, 10);
    EXPECT_EQ(output.lastPayload, R"({"state": "ON", "brightness": 10})");
}

TEST(LightDeviceSendTest, SendsOffCommand) {
    EventBus evbus;
    LightDevice device("kitchen_light", evbus);
    MockOutput output;

    device.send(output, false, 0);

    EXPECT_EQ(output.lastTopic, "zigbee2mqtt/kitchen_light/set");
    EXPECT_EQ(output.lastPayload, R"({"state": "OFF", "brightness": 0})");
}

TEST(LightDeviceEdgeCaseTest, ThrowsWhenMissingBrightness) {
    EventBus evbus;
    LightDevice device("living_light", evbus);

    nlohmann::json payload = {
        {"state", "ON"}
    };

    EXPECT_THROW(device.onMessage("living_light", payload), std::exception);
}

TEST(LightDeviceEdgeCaseTest, ThrowsWhenMissingState) {
    EventBus evbus;
    LightDevice device("living_light", evbus);

    nlohmann::json payload = {
        {"brightness", 100}
    };

    EXPECT_THROW(device.onMessage("living_light", payload), std::exception);
}

TEST(LightDeviceEdgeCaseTest, SendsWithOutOfRangeBrightness) {
    EventBus evbus;
    LightDevice device("living_light", evbus);
    MockOutput output;

    device.send(output, true, -10);
    EXPECT_EQ(output.lastTopic, "zigbee2mqtt/living_light/set");
    EXPECT_EQ(output.lastPayload, R"({"state": "ON", "brightness": -10})");

    device.send(output, true, 1000);
    EXPECT_EQ(output.lastPayload, R"({"state": "ON", "brightness": 1000})");
}

TEST(LightDeviceExtendedTest, ProcessesOnStateWithZeroBrightness) {
    const std::string deviceName = "office_light";
    EventBus evbus;
    MockListener listener(evbus);
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", "ON"},
        {"brightness", 0}
    };

    device.onMessage(deviceName, payload);

    EXPECT_TRUE(listener.notified);
    EXPECT_EQ(listener.brightness, 0);
}

TEST(LightDeviceExtendedTest, IgnoresExtraFields) {
    const std::string deviceName = "office_light";
    EventBus evbus;
    MockListener listener(evbus);
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", "ON"},
        {"brightness", 42},
        {"color_temp", 300},
        {"transition", 2}
    };

    device.onMessage(deviceName, payload);

    EXPECT_TRUE(listener.notified);
    EXPECT_EQ(listener.brightness, 42);
}

TEST(LightDeviceExtendedTest, ThrowsWhenMissingStateAndBrightness) {
    const std::string deviceName = "office_light";
    EventBus evbus;
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"foo", "bar"}
    };

    EXPECT_THROW(device.onMessage(deviceName, payload), std::exception);
}

TEST(LightDeviceExtendedTest, ThrowsForStringBrightness) {
    const std::string deviceName = "office_light";
    EventBus evbus;
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", "ON"},
        {"brightness", "high"}
    };

    EXPECT_THROW(device.onMessage(deviceName, payload), std::exception);
}

TEST(LightDeviceExtendedTest, ThrowsForNumericState) {
    const std::string deviceName = "office_light";
    EventBus evbus;
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", 123},
        {"brightness", 10}
    };

    EXPECT_THROW(device.onMessage(deviceName, payload), std::exception);
}

TEST(LightDeviceExtendedTest, OffStateYieldsZeroBrightnessEvent) {
    const std::string deviceName = "office_light";
    EventBus evbus;
    MockListener listener(evbus);
    LightDevice device(deviceName, evbus);

    nlohmann::json payload = {
        {"state", "OFF"},
        {"brightness", 999}
    };

    device.onMessage(deviceName, payload);

    EXPECT_TRUE(listener.notified);
    EXPECT_EQ(listener.brightness, 0);
}

}  // namespace device