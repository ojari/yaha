#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "../common.hpp"
#include "../data/data.hpp"
#include "../event_bus.hpp"

namespace device {

class Device {
public:
    Device(const std::string& name, EventBus& evbus) :
        deviceName(name),
		evbus(evbus)
    {}

    virtual ~Device() = default;

    virtual void onMessage(const std::string& deviceName, nlohmann::json& payload) = 0;

protected:
    bool str2bool(const std::string& str) const {
        return str == "ON";
    }

    std::string deviceName;
	EventBus& evbus;
};

}  // namespace device
