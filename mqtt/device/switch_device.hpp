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

        if (deviceName != _deviceName) { // this message was not for me
            return;
        }

        if (payload.contains("state")) {
            const auto& j = payload["state"];
            if (j.is_string()) {
                state = j.get<std::string>() == "ON";
            } else if (j.is_boolean()) {
                state = j.get<bool>();
            } else if (j.is_number_integer()) {
                state = j.get<int>() != 0;
            } else if (j.is_null()) {
                spdlog::warn("Switch {}: 'state' is null, ignoring", deviceName);
                return;
            } else {
                spdlog::warn("Switch {}: unexpected type for 'state'", deviceName);
                return;
            }
        } else if (payload.contains("action")) {
            const auto& j = payload["action"];
            if (j.is_string()) {
                const std::string s = j.get<std::string>();
                if (s == "on") state = true;
                else if (s == "off") state = false;
                else {
                    spdlog::warn("Switch {}: unknown action '{}'", deviceName, s);
                    return;
                }
            } else if (j.is_boolean()) {
                state = j.get<bool>();
            } else if (j.is_null()) {
                spdlog::warn("Switch {}: 'action' is null, ignoring", deviceName);
                return;
            } else {
                spdlog::warn("Switch {}: unexpected type for 'action'", deviceName);
                return;
            }
        } else {
            // No relevant field found
            return;
        }

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
