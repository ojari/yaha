#include <spdlog/spdlog.h>
#ifndef WIN32
#include <unistd.h>  // getcwd
#endif
#include <algorithm>
#include <array>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include "registry.hpp"
#include "light_device.hpp"
#include "shelly_device.hpp"
#include "switch_device.hpp"
#include "temp_sensor_device.hpp"
#include "debug_device.h"
#include <nlohmann/json.hpp>

namespace device {

namespace {

template <typename T>
std::shared_ptr<Device> makeDevice(const std::string& name, EventBus& evbus) {
    return std::make_shared<T>(name, evbus);
}

struct DeviceDescriptor {
    std::string_view type;
    std::shared_ptr<Device> (*create)(const std::string&, EventBus&);
};

constexpr std::array<DeviceDescriptor, 5> deviceDescriptors{ {
    {"Light", &makeDevice<LightDevice>},
    {"Switch", &makeDevice<SwitchDevice>},
    {"Shelly", &makeDevice<ShellyDevice>},
    {"TempSensor", &makeDevice<TempSensorDevice>},
    {"Debug", &makeDevice<DebugDevice>}
} };

}  // namespace

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
            const std::string type = deviceData["type"].get<std::string>();

            devices_[name] = createDevice(name, type, eventBus);
        }

        ifile.close();
    } else {
        spdlog::error("Error opening {}", filename);
    }
}

void Registry::route(const std::string& topic, const std::string& payload) {
    auto device = getDevice(topic);
    if (device) {
        if (0) {
            spdlog::info("Device: {} Payload: {}", topic, payload);
        }
        try {
            nlohmann::json jsonPayload = nlohmann::json::parse(payload);
            device->onMessage(topic, jsonPayload);
        } catch (const nlohmann::json::parse_error& e) {
            spdlog::error("Json parse error: {}", e.what());
            spdlog::error("Device: {} Payload: {}", topic, payload);
        } catch (const nlohmann::json::type_error& e) {
            spdlog::error("Json type error: {}", e.what());
            spdlog::error("Device: {} Payload: {}", topic, payload);
        }
    } else {
        spdlog::error("Device not found for topic: {} Payload: {}", topic, payload);
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
    EventBus& eventBus) const
{
    const auto it = std::find_if(
        deviceDescriptors.begin(),
        deviceDescriptors.end(),
        [&](const DeviceDescriptor& descriptor) {
            return descriptor.type == type;
        });

    if (it != deviceDescriptors.end()) {
        return it->create(name, eventBus);
    }

    spdlog::error("Unknown device type: {}", type);
    return nullptr;
}

}  // namespace device
