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
    MockOutput output;
};

TEST_F(ShellyDeviceTest, HandlesIncomingOnState) {
    nlohmann::json payload;
    payload["state"] = "ON";
    std::string deviceName = "test_dev";

    EXPECT_NO_THROW(shelly.onMessage(deviceName, payload));
}

TEST_F(ShellyDeviceTest, SendsOnMethodPayload) {
    shelly.send(output, true);
    EXPECT_EQ(output.lastTopic, "test_dev/rpc");
    EXPECT_EQ(output.lastPayload, R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":1, "on":true}})");
}

TEST_F(ShellyDeviceTest, SendsOffMethodPayload) {
    shelly.send(output, false);
    EXPECT_EQ(output.lastTopic, "test_dev/rpc");
    EXPECT_EQ(output.lastPayload, R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":1, "on":false}})");
}
