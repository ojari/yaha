#pragma once
#include <spdlog/spdlog.h>
#include "device.hpp"

namespace device {

class TempSensorDevice : public Device {
public:
    explicit TempSensorDevice(const std::string& name, EventId eid, std::shared_ptr<IEventBus> evbus) :
        Device(name, eid, evbus)
    {}

    void onMessage(std::string& deviceName, nlohmann::json& payload) override {
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

}  // namespace device
