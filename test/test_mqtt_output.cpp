#include <memory>
#include <string>
#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "../mqtt/automation/mqtt_output.hpp"
#include "../mqtt/device/registry.hpp"
#include "mocks.h"

namespace {

std::string writeDevicesConfig(const std::string& filename) {
    const auto path = std::filesystem::temp_directory_path() / filename;
    std::ofstream out(path);
    out << R"([
        {"type":"Switch","name":"zigbee_switch","event":"unknown"},
        {"type":"Shelly","name":"shelly_switch:2","event":"unknown"}
    ])";
    return path.string();
}

}  // namespace

TEST(MqttAutomationOutputTest, UsesZigbeeTopicAndPayloadForNonShellyDevice) {
    const std::string devicesFile = writeDevicesConfig("yaha_test_devices_1.json");
    EventBus evbus;
    device::Registry registry(evbus);
    registry.load(devicesFile);

    auto output = std::make_shared<MockOutput>();
    automation::MqttAutomationOutput mqttOutput(output, &registry);

    mqttOutput.sendCommand("zigbee_switch", { true, std::nullopt });

    EXPECT_EQ(output->lastTopic, "zigbee2mqtt/zigbee_switch/set");
    EXPECT_EQ(output->lastPayload, R"({"state": "ON"})");
}

TEST(MqttAutomationOutputTest, UsesShellyTopicAndPayloadForShellyDevice) {
    const std::string devicesFile = writeDevicesConfig("yaha_test_devices_2.json");
    EventBus evbus;
    device::Registry registry(evbus);
    registry.load(devicesFile);

    auto output = std::make_shared<MockOutput>();
    automation::MqttAutomationOutput mqttOutput(output, &registry);

    mqttOutput.sendCommand("shelly_switch:2", { false, std::nullopt });

    EXPECT_EQ(output->lastTopic, "shelly_switch/rpc");

    const auto payload = nlohmann::json::parse(output->lastPayload);
    EXPECT_EQ(payload["id"], 1);
    EXPECT_EQ(payload["src"], "mytopic");
    EXPECT_EQ(payload["method"], "Switch.Set");
    EXPECT_EQ(payload["params"]["id"], 2);
    EXPECT_EQ(payload["params"]["on"], false);
}
