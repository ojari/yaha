#include <memory>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "../task.hpp"
#include "registry.hpp"
#include "lights.hpp"
#include "car_heater.hpp"
#include "storage_heater.hpp"
#include "water_heater.hpp"
#include "room_heater.hpp"
#include "switch_light.hpp"

namespace automation {

void Registry::load(const std::string& filename, IEventManager& evman) {

    std::ifstream ifile(filename);
    if (ifile.is_open()) {
        nlohmann::json data;
        ifile >> data;

        for (const auto& automationData : data) {
            std::string name = automationData["name"].get<std::string>();
            const std::string type = automationData["type"].get<std::string>();

            std::vector<int> args;
            if (automationData.contains("arg") && automationData["arg"].is_array()) {
                for (const auto& arg : automationData["arg"]) {
                    args.push_back(arg.get<int>());
                }
            }

            auto ctrl = create(name, toAutomationType(type), args);
            ctrl->registerEvents(evman);
            controllers.push_back(ctrl);
        }

        ifile.close();
    } else {
        spdlog::error("Error opening {}", filename);
    }
}

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
    const std::string& name,
    AutomationType type,
    const std::vector<int>& args) const
{
    using CreateFunc = std::shared_ptr<Automation>(*)(const std::string&, std::shared_ptr<IActuator>, const std::vector<int>&);

    static const std::pair<AutomationType, CreateFunc> createMap[] = {
        {AutomationType::CAR_HEATER, [](const std::string& name, std::shared_ptr<IActuator> actuator, const std::vector<int>& args) {
            return CarHeater::create(name, actuator, args[0]);
        }},
        {AutomationType::LIGHTS, [](const std::string& name, std::shared_ptr<IActuator> actuator, const std::vector<int>& args) {
            return Lights::create(name, actuator, args[0], args[1]);
        }},
        {AutomationType::SWITCH, [](const std::string& name, std::shared_ptr<IActuator> actuator, const std::vector<int>&) {
            return SwitchLight::create(name, actuator);
        }},
        {AutomationType::STORAGE_HEATER, [](const std::string& name, std::shared_ptr<IActuator> actuator, const std::vector<int>&) {
            return StorageHeater::create(name, actuator);
        }},
        {AutomationType::WATER_HEATER, [](const std::string& name, std::shared_ptr<IActuator> actuator, const std::vector<int>&) {
            return WaterHeater::create(name, actuator);
        }},
        {AutomationType::ROOM_HEATER, [](const std::string& name, std::shared_ptr<IActuator> actuator, const std::vector<int>&) {
            return RoomHeater::create(name, actuator);
        }}
    };

    for (const auto& pair : createMap) {
        if (pair.first == type) {
            return pair.second(name, actuator, args);
        }
    }

    // Handle unknown type
    spdlog::error("Unknown AutomationType: {}", (int)type);
    return nullptr;
}

}