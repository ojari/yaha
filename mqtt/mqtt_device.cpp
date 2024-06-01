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
            const std::string type = deviceData["type"].get<std::string>();

            devices_[name] = createDevice(name, type);
            // std::cout << "Device: " << name << " Type: " << type << std::endl;
        }

        ifile.close();
    } else {
        showError("Error opening devices.json");
    }
}


std::shared_ptr<Device> DeviceRegistry::createDevice(const std::string& name, const std::string& type) const {
    static const std::map<std::string, std::function<std::shared_ptr<Device>()>> deviceMap = {
        {"Light", [name]() { return std::make_shared<LightDevice>(name); }},
        {"Switch", [name]() { return std::make_shared<SwitchDevice>(name); }},
        {"TempSensor", []() { return std::make_shared<TempSensorDevice>(); }}
    };

    auto it = deviceMap.find(type);
    if (it != deviceMap.end()) {
        return std::move((it->second)());
    } else {
        return nullptr;
    }
}
