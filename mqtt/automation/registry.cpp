#include <memory>
#include "../task.hpp"
#include "registry.hpp"
#include "lights.hpp"
#include "car_heater.hpp"
#include "storage_heater.hpp"
#include "water_heater.hpp"
#include "room_heater.hpp"
#include "switch_light.hpp"

namespace automation {

AutomationType Registry::toAutomationType(const std::string& typeStr)
{
    static const std::pair<std::string, AutomationType> typeMap[] = {
        {"LIGHTS", AutomationType::LIGHTS},
        {"SWITCH", AutomationType::SWITCH},
        {"CAR_HEATER", AutomationType::CAR_HEATER},
        {"STORAGE_HEATER", AutomationType::STORAGE_HEATER},
        {"WATER_HEATER", AutomationType::WATER_HEATER},
        {"ROOM_HEATER", AutomationType::ROOM_HEATER}
    };

    for (const auto& pair : typeMap) {
        if (pair.first == typeStr) {
            return pair.second;
        }
    }
    throw std::invalid_argument("Unknown AutomationType: " + typeStr);
}

std::shared_ptr<Automation> Registry::create(
    ITaskManager& tasks,
    const std::string_view name,
    AutomationType type,
    std::shared_ptr<IActuator> actuator,
    const int& arg1,
    const int& arg2) const
{
    if (type == AutomationType::CAR_HEATER) {
        return CarHeater::create(tasks, name, actuator, arg1);
    }
    else if (type == AutomationType::LIGHTS) {
        return Lights::create(tasks, name, actuator, arg1, arg2);
    }
    else if (type == AutomationType::SWITCH) {
        return StorageHeater::create(tasks, name, actuator);
    }
    else if (type == AutomationType::STORAGE_HEATER) {
        return SwitchLight::create(tasks, name, actuator);
    }
    else if (type == AutomationType::WATER_HEATER) {
        return WaterHeater::create(tasks, name, actuator);
    }
    else if (type == AutomationType::ROOM_HEATER) {
        return RoomHeater::create(tasks, name, actuator);
    }

    // Handle unknown type
    return nullptr;
}

}