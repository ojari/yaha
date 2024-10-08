#pragma once
#include "device.hpp"
#include <iostream>

namespace device {

class TempSensorDevice : public Device {
public:
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        temperature = payload["temperature"];
        humidity = payload["humidity"];
        std::cout << "Temperature " << deviceName << " :: " << temperature << "C :: " << humidity << "%" << std::endl;
    }
    void getHistory(DataHistory &history) override {
        history.type = DataType::TEMPERATURE;
        history.val1 = static_cast<int>(temperature);
        history.val2 = static_cast<int>(humidity);
    }
private:
    float temperature;
    float humidity;
};

}
