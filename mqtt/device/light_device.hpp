#pragma once
#include <spdlog/spdlog.h>
#include "../config.hpp"
#include "device.hpp"

namespace device {

class LightDevice : public Device, public IDeviceLightOut {
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

    void send(IOutput& output, bool value, int newBrightness) override {
        std::string topic = std::string(MQTT_TOPIC) + "/" + deviceName + "/set";
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
