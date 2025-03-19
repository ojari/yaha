#pragma once
#include <spdlog/spdlog.h>
#include "device.hpp"

namespace device {

class LightDevice : public Device, public IDeviceLightOut {
public:
    explicit LightDevice(const std::string& name, EventId eid) :
        Device(name, eid)
    {
    }

    void on_message(std::string& devName, nlohmann::json& payload) override {
        brightness = payload["brightness"];
        if (payload["state"].is_boolean()) {
            state = payload["state"];
        } else {
            state = str2bool(payload["state"]);
        }
        spdlog::info("Light  {} :: {} :: {}", devName, state, brightness);

        if (state == false) {
            notifyValue(0);
        } else {
            notifyValue(brightness);
        }
    }

    void send(IOutput& output, bool value, int newBrightness) override {
        std::string topic = "zigbee2mqtt/" + deviceName + "/set";
        std::string payload;
        payload.append(R"({"state": ")");
        payload.append(value ? "ON" : "OFF");
        payload.append(R"(", "brightness": )");
        payload.append(std::to_string(newBrightness));
        payload.append("}");
        output.send(topic, payload);
    }

private:
    int brightness = 0;
    bool state = false;
};

}  // namespace device
