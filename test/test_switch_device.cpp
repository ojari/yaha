#include "catch2/catch_all.hpp"
#include "../mqtt/device/switch_device.hpp"
#include <nlohmann/json.hpp>

namespace device {


TEST_CASE("SwitchDevice - on_message with state key") {
    MockEventListener listener;
    SwitchDevice device("test_switch", EventId::TEMPERATURE);
    device.subscribe(listener);

    SECTION("State ON") {
        nlohmann::json payload = {{"state", "ON"}};
        std::string deviceName = "test_switch";

        device.on_message(deviceName, payload);

        REQUIRE(listener.notified);
        REQUIRE(listener.lastValue == 1);
    }

    SECTION("State OFF") {
        nlohmann::json payload = {{"state", "OFF"}};
        std::string deviceName = "test_switch";

        device.on_message(deviceName, payload);

        REQUIRE(listener.notified);
        REQUIRE(listener.lastValue == 0);
    }
}

TEST_CASE("SwitchDevice - on_message with action key") {
    MockEventListener listener;
    SwitchDevice device("test_switch", EventId::TEMPERATURE);
    device.subscribe(listener);

    SECTION("Action on") {
        nlohmann::json payload = {{"action", "on"}};
        std::string deviceName = "test_switch";

        device.on_message(deviceName, payload);

        REQUIRE(listener.notified);
        REQUIRE(listener.lastValue == 1);
    }

    SECTION("Action off") {
        nlohmann::json payload = {{"action", "off"}};
        std::string deviceName = "test_switch";

        device.on_message(deviceName, payload);

        REQUIRE(listener.notified);
        REQUIRE(listener.lastValue == 0);
    }

    SECTION("Invalid action") {
        nlohmann::json payload = {{"action", "invalid"}};
        std::string deviceName = "test_switch";

        listener.notified = false;
        device.on_message(deviceName, payload);

        // Should not notify on invalid action
        REQUIRE_FALSE(listener.notified);
    }

    SECTION("Null action") {
        nlohmann::json payload = {{"action", nullptr}};
        std::string deviceName = "test_switch";

        listener.notified = false;
        device.on_message(deviceName, payload);

        // Should not notify on null action
        REQUIRE_FALSE(listener.notified);
    }
}

TEST_CASE("SwitchDevice - send method") {
    SwitchDevice device("test_switch", EventId::TEMPERATURE);
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
    MockEventListener listener;
    SwitchDevice device("test_switch", EventId::TEMPERATURE);
    device.subscribe(listener);

    nlohmann::json payload = {{"action", "off"}};
    std::string deviceName = "test_switch";

    device.on_message(deviceName, payload);

    // Current implementation has a bug where it overwrites state with payload["action"] == "on"
    // after already setting it based on the string value
    REQUIRE(listener.lastValue == 0);  // This might fail with current implementation
}

}  // namespace device
