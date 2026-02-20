#pragma once
#include <optional>
#include <utility>
#include <string>
#include <nlohmann/json.hpp>
#include "../common.hpp"
#include "../data/data.hpp"
#include "../event_bus.hpp"
#include "../config.hpp"

namespace device {

class Device {
public:
    Device(const std::string& name, EventBus& evbus) :
        deviceName(name),
		evbus(evbus)
    {}

    virtual ~Device() = default;

    virtual void onMessage(const std::string& deviceName, nlohmann::json& payload) = 0;

    virtual std::string buildCommandTopic() const {
        return std::string(MQTT_TOPIC) + "/" + deviceName + "/set";
    }

    virtual std::string buildCommandPayload(
        bool on,
        std::optional<int> brightness = std::nullopt) const
    {
        std::string payload = "{\"state\": \"";
        payload.append(on ? "ON" : "OFF");
        payload.append("\"");
        if (brightness.has_value()) {
            payload.append(", \"brightness\": ");
            payload.append(std::to_string(*brightness));
        }
        payload.append("}");
        return payload;
    }

protected:
    bool str2bool(const std::string& str) const {
        return str == "ON";
    }

    std::string deviceName;
	EventBus& evbus;
};

}  // namespace device
