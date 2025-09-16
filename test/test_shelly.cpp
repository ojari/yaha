#include <memory>
#include "catch2/catch_all.hpp"
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

TEST_CASE("ShellyDevice class test") {
    EventBus evbus;

    device::ShellyDevice shelly("test_dev", evbus);
    MockOutput output;
    SECTION("Turn on switch") {
        nlohmann::json payload;
        payload["state"] = "ON";
        std::string deviceName = "test_dev";
        shelly.onMessage(deviceName, payload);
        // REQUIRE(shelly.get() == true);
    }
    SECTION("Send ON method") {
        shelly.send(output, true);
        REQUIRE(output.lastTopic == "test_dev/rpc");
        REQUIRE(output.lastPayload == R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":1, "on":true}})");
    }
    SECTION("Send OFF method") {
        shelly.send(output, false);
        REQUIRE(output.lastTopic == "test_dev/rpc");
        REQUIRE(output.lastPayload == R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":1, "on":false}})");
    }
}
