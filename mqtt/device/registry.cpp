#include "registry.hpp"
#include "light_device.hpp"
#include "switch_device.hpp"
#include "temp_sensor_device.hpp"
// #include "common.hpp"
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream> // std::cout
#include <unistd.h> // getcwd

namespace device {

void Registry::load(const std::string& filename) {

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    }
    std::ifstream ifile(filename);
    if (ifile.is_open()) {
        nlohmann::json data;
        ifile >> data;

        for (const auto& deviceData : data) {
            std::string name = deviceData["name"].get<std::string>();
            std::string event = deviceData["event"].get<std::string>();
            const std::string type = deviceData["type"].get<std::string>();

            devices_[name] = createDevice(name, type, event);
        }

        ifile.close();
    } else {
        showError("Error opening devices.json");
    }
}

bool Registry::subscribe(EventId eventId, IObserver& observer) {
    for (const auto& [name, device] : devices_) {
        if (device->hasEvent(eventId)) {
            device->subscribe(observer);
            return true;
        }
    }
    return false;
}

std::shared_ptr<Device> Registry::createDevice(
    const std::string& name, 
    const std::string& type, 
    const std::string& eventStr) const
{
    static const std::map<std::string, std::function<std::shared_ptr<Device>(const std::string&, EventId event)>> deviceMap = {
        {"Light", [](const std::string& name, EventId event) { 
            return std::make_shared<LightDevice>(name, event); }
        },
        {"Switch", [](const std::string& name,EventId event) {
            return std::make_shared<SwitchDevice>(name, event); }
        },
        {"TempSensor", [](const std::string& name, EventId event) {
            return std::make_shared<TempSensorDevice>(name, event); }
        }
    };

    auto it = deviceMap.find(type);
    if (it != deviceMap.end()) {
        EventId event = str2event(eventStr);
        return std::move((it->second)(name, event));
    } else {
        return nullptr;
    }
}

}