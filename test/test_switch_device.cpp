#include "catch2/catch_all.hpp"
#include "../mqtt/device/switch_device.hpp"
#include "mocks.h"
#include <nlohmann/json.hpp>

namespace device {


TEST_CASE("SwitchDevice - on_message with state key") {
	auto evbus = std::make_shared<MockEventBus>();
    SwitchDevice device("test_switch", EventId::TEMPERATURE, evbus);

    SECTION("State ON") {
        nlohmann::json payload = {{"state", "ON"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(evbus->notified);
        REQUIRE(evbus->lastValue == 1);
    }

    SECTION("State OFF") {
        nlohmann::json payload = {{"state", "OFF"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(evbus->notified);
        REQUIRE(evbus->lastValue == 0);
    }
}

TEST_CASE("SwitchDevice - on_message with action key") {
	auto evbus = std::make_shared<MockEventBus>();
    SwitchDevice device("test_switch", EventId::TEMPERATURE, evbus);

    SECTION("Action on") {
        nlohmann::json payload = {{"action", "on"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(evbus->notified);
        REQUIRE(evbus->lastValue == 1);
    }

    SECTION("Action off") {
        nlohmann::json payload = {{"action", "off"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(evbus->notified);
        REQUIRE(evbus->lastValue == 0);
    }

    SECTION("Invalid action") {
        nlohmann::json payload = {{"action", "invalid"}};
        std::string deviceName = "test_switch";

        evbus->notified = false;
        device.onMessage(deviceName, payload);

        // Should not notify on invalid action
        REQUIRE_FALSE(evbus->notified);
    }

    SECTION("Null action") {
        nlohmann::json payload = {{"action", nullptr}};
        std::string deviceName = "test_switch";

        evbus->notified = false;
        device.onMessage(deviceName, payload);

        // Should not notify on null action
        REQUIRE_FALSE(evbus->notified);
    }
}

TEST_CASE("SwitchDevice - send method") {
	auto evbus = std::make_shared<EventBus>();
    SwitchDevice device("test_switch", EventId::TEMPERATURE, evbus);
    MockOutput output;

    SECTION("Send ON") {
        device.send(output, true);

        REQUIRE(output.lastTopic == "zigbee2mqtt/test_switch/set");
        REQUIRE(output.lastPayload == "{\"state\": \"ON\"}");
    }

    SECTION("Send OFF") {
        device.send(output, false);

        REQUIRE(output.lastTopic == "zigbee2mqtt/test_switch/set");
        REQUIRE(output.lastPayload == "{\"state\": \"OFF\"}");
    }
}

// Bug detection test - this test exposes a bug in the current implementation
TEST_CASE("SwitchDevice - bug in on_message") {
	auto evbus = std::make_shared<MockEventBus>();
    SwitchDevice device("test_switch", EventId::TEMPERATURE, evbus);

    nlohmann::json payload = {{"action", "off"}};
    std::string deviceName = "test_switch";

    device.onMessage(deviceName, payload);

    // Current implementation has a bug where it overwrites state with payload["action"] == "on"
    // after already setting it based on the string value
    REQUIRE(evbus->lastValue == 0);  // This might fail with current implementation
}

}  // namespace device
