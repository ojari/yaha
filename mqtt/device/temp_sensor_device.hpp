#pragma once
#include "device.hpp"
#include <iostream>

namespace device {

class TempSensorDevice : public Device {
public:
    explicit TempSensorDevice(const std::string& name, EventId eid) :
        Device(name, eid)
    {}

    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        temperature = payload["temperature"];
        humidity = payload["humidity"];
        std::cout << "Temperature " << deviceName << " :: " << temperature << "C :: " << humidity << "%" << std::endl;

        notifyValue(temperature);
    }

private:
    float temperature;
    float humidity;
};

}
