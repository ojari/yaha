#pragma once
#include <spdlog/spdlog.h>
#include "device.hpp"
#include "../events.h"

namespace device {

class DebugDevice : public Device, public IDeviceBoolOut {
public:
    explicit DebugDevice(const std::string& name, EventBus& evbus) :
        Device(name, evbus),
        evbus(evbus)
    {}

    void onMessage(std::string& _deviceName, nlohmann::json& payload) override {
        if (!payload.contains("event") || !payload["event"].is_string()) {
            spdlog::warn("DebugDevice: Payload missing 'event' field or it is not a string");
            return;
        }

        std::string eventType = payload["event"].get<std::string>();

        if (eventType == "Temperature" && payload.contains("room") && payload.contains("value")) {
            evbus.publish(TemperatureEvent(
                payload["room"].get<std::string>(),
                payload["value"].get<float>()));
        }
        else if (eventType == "Dark" && payload.contains("isDark")) {
            evbus.publish(DarkEvent(
                payload["isDark"].get<bool>()));
        }
        else if (eventType == "Date" && payload.contains("year") && payload.contains("month") && payload.contains("day") && payload.contains("weekday")) {
            evbus.publish(DateEvent(
                payload["year"].get<int>(),
                payload["month"].get<int>(),
                payload["day"].get<int>(),
                payload["weekday"].get<int>()));
        }
        else if (eventType == "Time" && payload.contains("hour") && payload.contains("minute")) {
            evbus.publish(TimeEvent(
                payload["hour"].get<int>(),
                payload["minute"].get<int>()));
        }
        else if (eventType == "Location" && payload.contains("latitude") && payload.contains("longitude")) {
            evbus.publish(LocationEvent(
                payload["latitude"].get<double>(),
                payload["longitude"].get<double>()));
        }
        else if (eventType == "ElectricityPrice" && payload.contains("price")) {
            evbus.publish(ElectricityPriceEvent(
                payload["price"].get<double>()));
        }
        else if (eventType == "Button" && payload.contains("location") && payload.contains("pressed")) {
            evbus.publish(ButtonEvent(
                payload["location"].get<std::string>(),
                payload["pressed"].get<bool>()));
        }
        else if (eventType == "Switch" && payload.contains("name") && payload.contains("state")) {
            evbus.publish(SwitchEvent(
                payload["name"].get<std::string>(),
                payload["state"].get<bool>()));
        }
        else if (eventType == "Lamp" && payload.contains("location") && payload.contains("brightness")) {
            evbus.publish(LampEvent(
                payload["location"].get<std::string>(),
                payload["brightness"].get<int>()));
        }
        else if (eventType == "Pc" && payload.contains("eventType") && payload.contains("value")) {
            PcEvent::EventClass pcEventType = PcEvent::process_memory;
            if (payload["eventType"] == "cpu_average") {
                pcEventType = PcEvent::cpu_average;
            }
            evbus.publish(PcEvent(pcEventType, payload["value"].get<unsigned long>()));
        }
        else {
            spdlog::warn("DebugDevice: Unknown event type or missing fields in payload");
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
    EventBus& evbus;
};

}  // namespace device
