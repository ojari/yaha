#pragma once
#include <spdlog/spdlog.h>
#include "device.hpp"

namespace device {

class DebugDevice : public Device, public IDeviceBoolOut {
public:
    explicit DebugDevice(const std::string& name, EventId eid, std::shared_ptr<IEventBus> evbus) :
        Device(name, eid, evbus),
        evbus(evbus)
    {}

    void onMessage(std::string& _deviceName, nlohmann::json& payload) override {
        EventId event = str2event(payload["event"]);
        if (payload["value"].is_number_float()) {
            float value = payload["value"].get<float>();
            evbus->publish(EventData(event, value));
        } else if (payload["value"].is_number_integer()) {
            int value = payload["value"].get<int>();
            evbus->publish(EventData(event, value));
        }
    }

    void send(IOutput& output, bool value) override {
        std::string topic = "debug/" + deviceName + "/set";
        std::string payload;
        // payload.append(R"({"state": ")");
        // payload.append(value ? "ON" : "OFF");
        // payload.append("\"}");
        output.send(topic, payload);
    }

private:
    std::shared_ptr<IEventBus> evbus;
};

}  // namespace device
