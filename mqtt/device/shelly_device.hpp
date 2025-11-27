#pragma once
#include <spdlog/spdlog.h>
#include "device.hpp"

namespace device {

/** The name is formatted like <devicename>:<relay number>
 */
std::string parseDeviceName(const std::string& name) {
    size_t pos = name.find(':');
    if (pos == std::string::npos) {
        return name;
    }
    return name.substr(0, pos);
}

int parseRelay(const std::string& name) {
    size_t pos = name.find(':');
    if (pos == std::string::npos) {
        return 0;
    }
    return std::stoi(name.substr(pos + 1));
}

class ShellyDevice : public Device, public IDeviceBoolOut {
public:
    explicit ShellyDevice(const std::string& name, EventBus& evbus) :
        Device(parseDeviceName(name), evbus),
        relay(parseRelay(name))
    {}

    void onMessage(const std::string& _deviceName, nlohmann::json& payload) override {
        /*if (payload.contains("state")) {
            state = payload["state"].get<std::string>() == "ON";
        }
        else {
            if (payload["action"].is_null()) {
                return;
            }
            const std::string& state_str = payload["action"].get<std::string>();
            if (state_str == "on") {
                state = true;
            } else if (state_str == "off") {
                state = false;
            }
            else {
                return;
            }
            state = payload["action"].get<std::string>() == "on";
        }
        spdlog::info("Shelly {} :: {}", deviceName, state);

        notifyValue((int)state);
        */
    }

    void send(IOutput& output, bool value) override {
        std::string topic = deviceName + "/rpc";
        std::string payload;
        payload.append(R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":1, "on":)");
        payload.append(value ? "true" : "false");
        payload.append("}}");
        output.send(topic, payload);
    }

private:
    bool state = false;
    int relay = 0;
};

}  // namespace device
