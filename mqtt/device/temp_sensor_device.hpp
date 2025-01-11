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
        float new_temperature = payload["temperature"];
        humidity = payload["humidity"];

        // spdlog::info("Temperature {} :: {}C :: {}%", deviceName, temperature, humidity);

        if (fabs(temperature - new_temperature) > 0.1) {
            temperature = new_temperature;
            notifyValue(temperature);
        }
    }

private:
    float temperature = -123.0;
    float humidity;
};

}
