#pragma once
#include <spdlog/spdlog.h>
#include "device.hpp"

namespace device {

class LightDevice : public Device {
public:
    explicit LightDevice(const std::string& name, EventBus& evbus) :
        Device(name, evbus)
    {
    }

    void onMessage(const std::string& devName, nlohmann::json& payload) override {
        brightness = payload["brightness"];
        if (payload["state"].is_boolean()) {
            state = payload["state"];
        } else {
            state = str2bool(payload["state"]);
        }
        // spdlog::info("Light  {} :: {} :: {}", devName, state, brightness);

        if (state == false) {
            evbus.publish(LampEvent(deviceName, 0));
        } else {
            evbus.publish(LampEvent(deviceName, brightness));
        }
    }

private:
    int brightness = 0;
    bool state = false;
};

}  // namespace device
