#pragma once
#include <spdlog/spdlog.h>
#include "device.hpp"
#include "../config.hpp"

namespace device {

class SwitchDevice : public Device, public IDeviceBoolOut {
public:
    explicit SwitchDevice(const std::string& name, EventBus& evbus) :
        Device(name, evbus)
    {}

    void reset() {
        first_time = true;
        last_state = false;
    }

    void onMessage(std::string& _deviceName, nlohmann::json& payload) override {
        bool state;

        if (payload.contains("state")) {
            state = payload["state"].get<std::string>() == "ON";
        } else {
            if (payload["action"].is_null()) {
                return;
            }
            const std::string& state_str = payload["action"].get<std::string>();
            if (state_str == "on") {
                state = true;
            } else if (state_str == "off") {
                state = false;
            } else {
                return;
            }
            state = payload["action"].get<std::string>() == "on";
        }
        // spdlog::info("Switch {} :: {}", deviceName, state);

        if (first_time || (state != last_state)) {
            evbus.publish<SwitchEvent>(SwitchEvent(deviceName, state));
            last_state = state;
            first_time = false;
        }
    }

    void send(IOutput& output, bool value) override {
        std::string topic = std::string(MQTT_TOPIC) + "/" + deviceName + "/set";
        std::string payload;
        payload.append(R"({"state": ")");
        payload.append(value ? "ON" : "OFF");
        payload.append("\"}");
        output.send(topic, payload);
    }

private:
    bool last_state = false;
    bool first_time = true;
};

}  // namespace device
