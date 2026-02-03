#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "../mqtt/device/switch_device.hpp"
#include "mocks.h"

namespace device {

struct MockSwitch {
    explicit MockSwitch(EventBus& evbus) {
        evbus.subscribe<SwitchEvent>([&](const SwitchEvent& e) {
            name = e.name;
            state = e.state;
            notified = true;
        });
    }

    std::string name;
    bool state = false;
    bool notified = false;
};

TEST(SwitchDeviceStateTest, HandlesOnStateMessages) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", "ON"}};
    device.onMessage("test_switch", payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_TRUE(mock.state);
}

TEST(SwitchDeviceStateTest, HandlesOffStateMessages) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", "OFF"}};
    device.onMessage("test_switch", payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_FALSE(mock.state);
}

TEST(SwitchDeviceActionTest, HandlesOnActions) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"action", "on"}};
    device.onMessage("test_switch", payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_TRUE(mock.state);
}

TEST(SwitchDeviceActionTest, HandlesOffActions) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"action", "off"}};
    device.onMessage("test_switch", payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_FALSE(mock.state);
}

TEST(SwitchDeviceActionTest, IgnoresInvalidActions) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"action", "invalid"}};
    mock.notified = false;
    device.onMessage("test_switch", payload);

    EXPECT_FALSE(mock.notified);
}

TEST(SwitchDeviceActionTest, IgnoresNullActions) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"action", nullptr}};
    mock.notified = false;
    device.onMessage("test_switch", payload);

    EXPECT_FALSE(mock.notified);
}

TEST(SwitchDeviceSendTest, PublishesOnPayload) {
    EventBus evbus;
    SwitchDevice device("test_switch", evbus);
    MockOutput output;

    device.send(output, true);

    EXPECT_EQ(output.lastTopic, "zigbee2mqtt/test_switch/set");
    EXPECT_EQ(output.lastPayload, "{\"state\": \"ON\"}");
}

TEST(SwitchDeviceSendTest, PublishesOffPayload) {
    EventBus evbus;
    SwitchDevice device("test_switch", evbus);
    MockOutput output;

    device.send(output, false);

    EXPECT_EQ(output.lastTopic, "zigbee2mqtt/test_switch/set");
    EXPECT_EQ(output.lastPayload, "{\"state\": \"OFF\"}");
}

TEST(SwitchDeviceActionTest, ExposesKnownBug) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"action", "off"}};
    device.onMessage("test_switch", payload);

    EXPECT_FALSE(mock.state);
}

TEST(SwitchDeviceRoutingTest, IgnoresOtherDevices) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", "ON"}};
    mock.notified = false;
    device.onMessage("other_device", payload);

    EXPECT_FALSE(mock.notified);
}

TEST(SwitchDeviceStateTest, AcceptsBooleanState) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", true}};
    mock.notified = false;
    device.onMessage("test_switch", payload);

    EXPECT_TRUE(mock.notified);
}

TEST(SwitchDeviceStateTest, IgnoresNullState) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", nullptr}};
    mock.notified = false;
    device.onMessage("test_switch", payload);

    EXPECT_FALSE(mock.notified);
}

TEST(SwitchDevicePreferenceTest, PrefersStateOverAction) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", "OFF"}, {"action", "on"}};
    device.onMessage("test_switch", payload);

    EXPECT_TRUE(mock.notified);
    EXPECT_FALSE(mock.state);

    nlohmann::json payload2 = {{"action", "off"}, {"state", "ON"}};
    mock.notified = false;
    device.onMessage("test_switch", payload2);

    EXPECT_TRUE(mock.notified);
    EXPECT_TRUE(mock.state);
}

TEST(SwitchDeviceSubscriptionTest, NotifiesMultipleSubscribers) {
    EventBus evbus;
    MockSwitch mock1(evbus);
    MockSwitch mock2(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", "ON"}};
    device.onMessage("test_switch", payload);

    EXPECT_TRUE(mock1.notified);
    EXPECT_TRUE(mock2.notified);
    EXPECT_TRUE(mock1.state);
    EXPECT_TRUE(mock2.state);
}

TEST(SwitchDeviceStateTest, EmptyPayloadDoesNotNotify) {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = nlohmann::json::object();
    mock.notified = false;
    device.onMessage("test_switch", payload);

    EXPECT_FALSE(mock.notified);
}

}  // namespace device
