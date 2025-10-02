#include "catch2/catch_all.hpp"
#include "../mqtt/device/light_device.hpp"
#include <nlohmann/json.hpp>
#include "mocks.h"

namespace device {
TEST_CASE("LightDevice construction", "[light]") {
    EventBus evbus;
    device::LightDevice device("bedroom_light", evbus);
    
    // Since the internals are private, we'll test indirectly via send method
    MockOutput output;
    device.send(output, true, 100);
    
    REQUIRE(output.lastTopic == "zigbee2mqtt/bedroom_light/set");
    REQUIRE(output.lastPayload.find("\"state\": \"ON\"") != std::string::npos);
    REQUIRE(output.lastPayload.find("\"brightness\": 100") != std::string::npos);
}

struct MockListener {
    MockListener(EventBus& evbus) {
        evbus.subscribe<LampEvent>([&](const LampEvent& e) {
            location = e.location;
            brightness = e.brightness;
            notified = true;
        });
    }

    std::string location;
    int brightness = -1;
    int notified = false;
};

TEST_CASE("LightDevice on_message method", "[light]") {
    std::string deviceName = "bedroom_light";
    EventBus evbus;
    MockListener mock(evbus);

    device::LightDevice device(deviceName, evbus);
    
    SECTION("Process ON state with brightness") {
        nlohmann::json payload = {
            {"state", "ON"},
            {"brightness", 150}
        };
        
        device.onMessage(deviceName, payload);
        
        REQUIRE(mock.notified > 0);
        REQUIRE(mock.brightness == 150);
    }
    
    SECTION("Process OFF state") {
        nlohmann::json payload = {
            {"state", "OFF"},
            {"brightness", 150}
        };
        
        device.onMessage(deviceName, payload);
        
        REQUIRE(mock.notified > 0);
        REQUIRE(mock.brightness == 0);  // Should notify 0 when state is OFF
    }
    
    SECTION("Process false state") {
        nlohmann::json payload = {
            {"state", false},
            {"brightness", 200}
        };
        
        device.onMessage(deviceName, payload);
        
        REQUIRE(mock.notified > 0);
        REQUIRE(mock.brightness == 0);
    }
}

TEST_CASE("LightDevice send method", "[light]") {
    EventBus evbus;
    device::LightDevice device("kitchen_light", evbus);
    MockOutput output;
    
    SECTION("Send ON command with brightness 100") {
        device.send(output, true, 100);
        
        REQUIRE(output.lastTopic == "zigbee2mqtt/kitchen_light/set");
        REQUIRE(output.lastPayload == R"({"state": "ON", "brightness": 100})");
    }
    
    SECTION("Send OFF command with brightness 0") {
        device.send(output, false, 0);
        
        REQUIRE(output.lastTopic == "zigbee2mqtt/kitchen_light/set");
        REQUIRE(output.lastPayload == R"({"state": "OFF", "brightness": 0})");
    }
    
    SECTION("Send ON command with max brightness") {
        device.send(output, true, 255);
        
        REQUIRE(output.lastTopic == "zigbee2mqtt/kitchen_light/set");
        REQUIRE(output.lastPayload == R"({"state": "ON", "brightness": 255})");
    }

    SECTION("Send ON command with low brightness") {
        device.send(output, true, 10);
        
        REQUIRE(output.lastTopic == "zigbee2mqtt/kitchen_light/set");
        REQUIRE(output.lastPayload == R"({"state": "ON", "brightness": 10})");
    }
}

TEST_CASE("LightDevice edge cases", "[light]") {
    EventBus evbus;
    device::LightDevice device("living_light", evbus);
    std::string deviceName = "living_light";
    MockOutput output;

    SECTION("Handle malformed JSON missing brightness") {
        nlohmann::json payload = {
            {"state", "ON"}
            // Missing brightness
        };
        
        // This should throw an exception due to missing brightness
        REQUIRE_THROWS(device.onMessage(deviceName, payload));
    }
    
    SECTION("Handle malformed JSON missing state") {
        nlohmann::json payload = {
            // Missing state
            {"brightness", 100}
        };
        
        // This should throw an exception due to missing state
        REQUIRE_THROWS(device.onMessage(deviceName, payload));
    }
    
    SECTION("Send with invalid brightness values") {
        // Test with negative brightness (implementation-dependent behavior)
        device.send(output, true, -10);
        
        REQUIRE(output.lastTopic == "zigbee2mqtt/living_light/set");
        REQUIRE(output.lastPayload == R"({"state": "ON", "brightness": -10})");
        
        // Test with extremely high brightness (implementation-dependent behavior)
        device.send(output, true, 1000);
        
        REQUIRE(output.lastTopic == "zigbee2mqtt/living_light/set");
        REQUIRE(output.lastPayload == R"({"state": "ON", "brightness": 1000})");
    }
}

//
// Additional improved coverage
//
TEST_CASE("LightDevice additional parsing cases", "[light][extended]") {
    std::string deviceName = "office_light";
    EventBus evbus;
    MockListener listener(evbus);
    device::LightDevice device(deviceName, evbus);

    SECTION("Process ON state with brightness 0") {
        nlohmann::json payload = {
            {"state", "ON"},
            {"brightness", 0}
        };
        device.onMessage(deviceName, payload);
        REQUIRE(listener.notified);
        REQUIRE(listener.brightness == 0);
    }

    SECTION("Process ON state with extra unrelated fields") {
        nlohmann::json payload = {
            {"state", "ON"},
            {"brightness", 42},
            {"color_temp", 300},
            {"transition", 2}
        };
        device.onMessage(deviceName, payload);
        REQUIRE(listener.notified);
        REQUIRE(listener.brightness == 42);
    }

    SECTION("Malformed JSON missing both state and brightness") {
        nlohmann::json payload = {
            {"foo", "bar"}
        };
        REQUIRE_THROWS(device.onMessage(deviceName, payload));
    }

    SECTION("Invalid brightness type (string)") {
        nlohmann::json payload = {
            {"state", "ON"},
            {"brightness", "high"}
        };
        REQUIRE_THROWS(device.onMessage(deviceName, payload));
    }

    SECTION("Invalid state type (number)") {
        nlohmann::json payload = {
            {"state", 123},
            {"brightness", 10}
        };
        REQUIRE_THROWS(device.onMessage(deviceName, payload));
    }

    SECTION("OFF state with large brightness still yields 0 event brightness") {
        nlohmann::json payload = {
            {"state", "OFF"},
            {"brightness", 999}
        };
        device.onMessage(deviceName, payload);
        REQUIRE(listener.notified);
        REQUIRE(listener.brightness == 0);
    }
}

}  // namespace device