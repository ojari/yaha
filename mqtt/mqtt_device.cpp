#include "mqtt_device.hpp"
#include "common.hpp"
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

void DeviceRegistry::load(const std::string& filename) {
    std::ifstream ifile(filename);
    if (ifile.is_open()) {
        nlohmann::json data;
        ifile >> data;

        for (const auto& deviceData : data) {
            std::string name = deviceData["name"].get<std::string>();
            std::string type = deviceData["type"].get<std::string>();

            if (type == "Lamp") {
                registerDevice(name, &lightDevice);
            } else if (type == "TempSensor") {
                registerDevice(name, &tempSensorDevice);
            } else if (type == "Switch") {
                registerDevice(name, &switchDevice);
            } else {
                showError("Unknown device type", type.c_str());
            }
            // std::cout << "Device: " << name << " Type: " << type << std::endl;
        }

        ifile.close();
    } else {
        showError("Error opening devices.json");
    }
}
