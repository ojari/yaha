#pragma once
#include "device.hpp"
#include <spdlog/spdlog.h>

namespace device {

class TempSensorDevice : public Device {
public:
    explicit TempSensorDevice(const std::string& name, EventId eid) :
        Device(name, eid)
    {}

    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        temperature = payload["temperature"];
        humidity = payload["humidity"];

        spdlog::info("Temperature {} :: {}C :: {}%", deviceName, temperature, humidity);

        notifyValue(temperature);
    }

private:
    float temperature;
    float humidity;
};

}
