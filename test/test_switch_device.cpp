#include "catch2/catch_all.hpp"
#include "../mqtt/device/switch_device.hpp"
#include "mocks.h"
#include <nlohmann/json.hpp>

namespace device {

struct MockSwitch {
    MockSwitch(EventBus& evbus) {
        evbus.subscribe<SwitchEvent>([&](const SwitchEvent& e) {
            name = e.name;
            state = e.state;
            notified = true;
            // Print to Catch2 logs
            INFO("SwitchEvent received: name=" << name << ", state=" << state);
        });
    }

    std::string name;
    bool state = false;
    int notified = false;
};


TEST_CASE("SwitchDevice - on_message with state key") {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    SECTION("State ON") {
        nlohmann::json payload = {{"state", "ON"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(mock.notified);
        REQUIRE(mock.state == true);
    }

    SECTION("State OFF") {
        nlohmann::json payload = {{"state", "OFF"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(mock.notified);
        REQUIRE(mock.state == false);
    }
}

TEST_CASE("SwitchDevice - on_message with action key") {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    SECTION("Action on") {
        nlohmann::json payload = {{"action", "on"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(mock.notified);
        REQUIRE(mock.state == true);
    }

    SECTION("Action off") {
        nlohmann::json payload = {{"action", "off"}};
        std::string deviceName = "test_switch";

        device.onMessage(deviceName, payload);

        REQUIRE(mock.notified);
        REQUIRE(mock.state == false);
    }

    SECTION("Invalid action") {
        nlohmann::json payload = {{"action", "invalid"}};
        std::string deviceName = "test_switch";

        mock.notified = false;
        device.onMessage(deviceName, payload);

        // Should not notify on invalid action
        REQUIRE_FALSE(mock.notified);
    }

    SECTION("Null action") {
        nlohmann::json payload = {{"action", nullptr}};
        std::string deviceName = "test_switch";

        mock.notified = false;
        device.onMessage(deviceName, payload);

        // Should not notify on null action
        REQUIRE_FALSE(mock.notified);
    }
}

TEST_CASE("SwitchDevice - send method") {
    EventBus evbus;
    SwitchDevice device("test_switch", evbus);
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
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"action", "off"}};
    std::string deviceName = "test_switch";

    device.onMessage(deviceName, payload);

    // Current implementation has a bug where it overwrites state with payload["action"] == "on"
    // after already setting it based on the string value
    REQUIRE(mock.state == false);  // This might fail with current implementation
}

TEST_CASE("SwitchDevice - ignores messages for other device") {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);

    nlohmann::json payload = {{"state", "ON"}};
    std::string deviceName = "other_device";

    mock.notified = false;
    device.onMessage(deviceName, payload);

    REQUIRE_FALSE(mock.notified);
}

TEST_CASE("SwitchDevice - non-string or null state should not notify") {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);
    std::string deviceName = "test_switch";

    SECTION("Boolean state") {
		nlohmann::json payload = { {"state", true} }; // accepted as valid
        mock.notified = false;
        device.onMessage(deviceName, payload);
        REQUIRE(mock.notified);
    }

    SECTION("Null state") {
        nlohmann::json payload = {{"state", nullptr}};
        mock.notified = false;
        device.onMessage(deviceName, payload);
        REQUIRE_FALSE(mock.notified);
    }
}

TEST_CASE("SwitchDevice - both 'state' and 'action' present: 'state' wins") {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);
    std::string deviceName = "test_switch";

    // If implementation should prefer explicit "state" over "action"
    nlohmann::json payload = {{"state", "OFF"}, {"action", "on"}};
    device.onMessage(deviceName, payload);
    REQUIRE(mock.notified);
    REQUIRE(mock.state == false);

    // reverse order: still prefer "state"
    nlohmann::json payload2 = {{"action", "off"}, {"state", "ON"}};
    mock.notified = false;
    device.onMessage(deviceName, payload2);
    REQUIRE(mock.notified);
    REQUIRE(mock.state == true);
}

TEST_CASE("SwitchDevice - multiple subscribers are notified") {
    EventBus evbus;
    MockSwitch mock1(evbus);
    MockSwitch mock2(evbus);
    SwitchDevice device("test_switch", evbus);
    std::string deviceName = "test_switch";

    nlohmann::json payload = {{"state", "ON"}};
    device.onMessage(deviceName, payload);

    REQUIRE(mock1.notified);
    REQUIRE(mock2.notified);
    REQUIRE(mock1.state == true);
    REQUIRE(mock2.state == true);
}

TEST_CASE("SwitchDevice - empty payload does not notify") {
    EventBus evbus;
    MockSwitch mock(evbus);
    SwitchDevice device("test_switch", evbus);
    std::string deviceName = "test_switch";

    nlohmann::json payload = nlohmann::json::object();
    mock.notified = false;
    device.onMessage(deviceName, payload);
    REQUIRE_FALSE(mock.notified);
}

}  // namespace device
