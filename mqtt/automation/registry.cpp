#include <spdlog/spdlog.h>
#include <memory>
#include <fstream>
#include <nlohmann/json.hpp>
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

            if (!type.empty() && type[0] == '#') {
                continue;
            }

            auto ctrl = create(name, toAutomationType(type));

            for (auto it = automationData.begin(); it != automationData.end(); ++it) {
                if (it.key() != "name" &&
                    it.key() != "type")
                {
                    if (it.value().is_string()) {
                        ctrl->setArg(it.key(), it.value().get<std::string>());
                    } else if (it.value().is_number()) {
                        ctrl->setArg(it.key(), std::to_string(it.value().get<int>()));
                    } else {
                        spdlog::warn("Expected string type for key: {}, but got different type", it.key());
                    }
                }
            }

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
    AutomationType type) const
{
    using CreateFunc = std::shared_ptr<Automation>(*)(const std::string&, std::shared_ptr<IOutput>);

    static const std::pair<AutomationType, CreateFunc> createMap[] = {
        {AutomationType::CAR_HEATER, [](const std::string& name, std::shared_ptr<IOutput> output) {
            return CarHeater::create(name, output);
        }},
        {AutomationType::LIGHTS, [](const std::string& name, std::shared_ptr<IOutput> output) {
            return Lights::create(name, output);
        }},
        {AutomationType::SWITCH, [](const std::string& name, std::shared_ptr<IOutput> output) {
            return SwitchLight::create(name, output);
        }},
        {AutomationType::STORAGE_HEATER, [](const std::string& name, std::shared_ptr<IOutput> output) {
            return StorageHeater::create(name, output);
        }},
        {AutomationType::WATER_HEATER, [](const std::string& name, std::shared_ptr<IOutput> output) {
            return WaterHeater::create(name, output);
        }},
        {AutomationType::ROOM_HEATER, [](const std::string& name, std::shared_ptr<IOutput> output) {
            return RoomHeater::create(name, output);
        }}
    };

    for (const auto& pair : createMap) {
        if (pair.first == type) {
            return pair.second(name, output);
        }
    }

    // Handle unknown type
    spdlog::error("Unknown AutomationType: {}", (int)type);
    return nullptr;
}

}  // namespace automation
