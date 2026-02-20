#pragma once
#include <optional>
#include <spdlog/spdlog.h>
#include "device.hpp"

namespace device {

/** The name is formatted like <devicename>:<relay number>
 */
inline std::string parseDeviceName(const std::string& name) {
    size_t pos = name.find(':');
    if (pos == std::string::npos) {
        return name;
    }
    return name.substr(0, pos);
}

inline int parseRelay(const std::string& name) {
    size_t pos = name.find(':');
    if (pos == std::string::npos) {
        return 0;
    }
    return std::stoi(name.substr(pos + 1));
}

class ShellyDevice : public Device {
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

    std::string buildCommandTopic() const override {
        return deviceName + "/rpc";
    }

    std::string buildCommandPayload(
        bool on,
        std::optional<int> brightness = std::nullopt) const override
    {
        std::string payload;
        payload.append(R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":)");
        payload.append(std::to_string(relay));
        payload.append(R"(, "on":)");
        payload.append(on ? "true" : "false");
        payload.append("}}");
        return payload;
    }

    /*void send(IOutput& output, bool value) override {
        std::string topic = deviceName + "/rpc";
        std::string payload;
        payload.append(R"({"id":1, "src":"mytopic", "method":"Switch.Set", "params": {"id":1, "on":)");
        payload.append(value ? "true" : "false");
        payload.append("}}");
        output.send(topic, payload);
    }*/

private:
    bool state = false;
    int relay = 0;
};

}  // namespace device
