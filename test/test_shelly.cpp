#include <memory>

#include <gtest/gtest.h>

#include "../mqtt/device/shelly_device.hpp"
#include "../mqtt/common.hpp"

class MockOutput : public IOutput {
public:
    void send(std::string_view topic, const std::string& payload) override {
        lastTopic = topic;
        lastPayload = payload;
    }

    std::string lastTopic;
    std::string lastPayload;
};

class ShellyDeviceTest : public ::testing::Test {
protected:
    EventBus evbus;
    device::ShellyDevice shelly {"test_dev", evbus};
};

TEST_F(ShellyDeviceTest, HandlesIncomingOnState) {
    nlohmann::json payload;
    payload["state"] = "ON";
    std::string deviceName = "test_dev";

    EXPECT_NO_THROW(shelly.onMessage(deviceName, payload));
}

TEST_F(ShellyDeviceTest, SendsOnMethodPayload) {
    EXPECT_EQ(shelly.buildCommandTopic(), "test_dev/rpc");
    EXPECT_EQ(shelly.buildCommandPayload(true), R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":0, "on":true}})");
}

TEST_F(ShellyDeviceTest, SendsOffMethodPayload) {
    EXPECT_EQ(shelly.buildCommandTopic(), "test_dev/rpc");
    EXPECT_EQ(shelly.buildCommandPayload(false), R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":0, "on":false}})");
}
