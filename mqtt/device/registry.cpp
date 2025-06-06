#include <spdlog/spdlog.h>
#ifndef WIN32
#include <unistd.h>  // getcwd
#endif
#include <fstream>
#include <string>
#include "registry.hpp"
#include "light_device.hpp"
#include "shelly_device.hpp"
#include "switch_device.hpp"
#include "temp_sensor_device.hpp"
#include "debug_device.h"
#include <nlohmann/json.hpp>

namespace device {

void Registry::load(const std::string& filename) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        spdlog::info("Current working directory: {}", cwd);
    }
    std::ifstream ifile(filename);
    if (ifile.is_open()) {
        nlohmann::json data;
        ifile >> data;

        for (const auto& deviceData : data) {
            std::string name = deviceData["name"].get<std::string>();
            std::string event = deviceData["event"].get<std::string>();
            const std::string type = deviceData["type"].get<std::string>();

            devices_[name] = createDevice(name, type, event, eventBus);
        }

        ifile.close();
    } else {
        spdlog::error("Error opening {}", filename);
    }
}

/*bool Registry::subscribe(EventId eventId, IObserver& observer) {
    for (const auto& [name, device] : devices_) {
        if (device->hasEvent(eventId)) {
            device->subscribe(observer);
            return true;
        }
    }
    return false;
}*/

std::shared_ptr<Device> Registry::createDevice(
    const std::string& name,
    const std::string& type,
    const std::string& eventStr,
    std::shared_ptr<IEventBus> eventBus) const
{
    static const std::map<std::string, std::function<std::shared_ptr<Device>(const std::string&, EventId event, std::shared_ptr<IEventBus> evbus)>> deviceMap = {
        {"Light", [](const std::string& name, EventId event, std::shared_ptr<IEventBus> evbus) {
            return std::make_shared<LightDevice>(name, event, evbus); }
        },
        {"Switch", [](const std::string& name, EventId event, std::shared_ptr<IEventBus> evbus) {
            return std::make_shared<SwitchDevice>(name, event, evbus); }
        },
        {"Shelly", [](const std::string& name, EventId event, std::shared_ptr<IEventBus> evbus) {
            return std::make_shared<ShellyDevice>(name, event, evbus); }
        },
        {"TempSensor", [](const std::string& name, EventId event, std::shared_ptr<IEventBus> evbus) {
            return std::make_shared<TempSensorDevice>(name, event, evbus); }
        },
        {"Debug", [](const std::string& name, EventId event, std::shared_ptr<IEventBus> evbus) {
            return std::make_shared<DebugDevice>(name, event, evbus); }
        }
    };

    auto it = deviceMap.find(type);
    if (it != deviceMap.end()) {
        EventId event = str2event(eventStr);
        return std::move((it->second)(name, event, eventBus));
    } else {
        spdlog::error("Unknown device type: {}", type);
        return nullptr;
    }
}

}  // namespace device
