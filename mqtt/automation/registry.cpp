#include <spdlog/spdlog.h>
#include <algorithm>
#include <array>
#include <memory>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../task.hpp"
#include "registry.hpp"
#include "lights.hpp"
#include "car_heater.hpp"
#include "storage_heater.hpp"
#include "switch_light.hpp"

namespace automation {

namespace {

using CreateFunc = std::shared_ptr<Automation>(*)(const std::string&, std::shared_ptr<IOutput>);

struct AutomationDescriptor {
    const char* typeName;
    AutomationType type;
    CreateFunc create;
};

constexpr std::array<AutomationDescriptor, 6> automationDescriptors{ {
    {"LIGHTS", AutomationType::LIGHTS, &Lights::create},
    {"SWITCH", AutomationType::SWITCH, &SwitchLight::create},
    {"CAR_HEATER", AutomationType::CAR_HEATER, &CarHeater::create},
    {"STORAGE_HEATER", AutomationType::STORAGE_HEATER, &StorageHeater::create}
} };

}  // namespace

void Registry::load(const std::string& filename, EventBus& evbus) {
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
                    if (it.key() == "conditions") {
                        for(auto condIt = it.value().begin(); condIt != it.value().end(); ++condIt) {
                            if (condIt->contains("type")) {
                                const std::string condType = (*condIt)["type"].get<std::string>();

                                ctrl->setCondition(condType, *condIt);
                            } else {
                                spdlog::warn("Condition is missing 'type'");
                            }
                        }
                    }
                    if (it.value().is_string()) {
                        ctrl->setArg(it.key(), it.value().get<std::string>());
                    } else if (it.value().is_number()) {
                        ctrl->setArg(it.key(), std::to_string(it.value().get<int>()));
                    } else {
                        spdlog::warn("Expected string type for key: {}, but got different type", it.key());
                    }
                }
            }

            ctrl->registerEvents(evbus);
            controllers.push_back(ctrl);
        }

        ifile.close();
    } else {
        spdlog::error("Error opening {}", filename);
    }
}

AutomationType Registry::toAutomationType(const std::string& typeStr)
{
    const auto it = std::find_if(
        automationDescriptors.begin(),
        automationDescriptors.end(),
        [&](const AutomationDescriptor& descriptor) {
            return typeStr == descriptor.typeName;
        });

    if (it != automationDescriptors.end()) {
        return it->type;
    }
    throw std::invalid_argument("Unknown AutomationType: " + typeStr);
}

std::shared_ptr<Automation> Registry::create(
    const std::string& name,
    AutomationType type) const
{
    const auto it = std::find_if(
        automationDescriptors.begin(),
        automationDescriptors.end(),
        [type](const AutomationDescriptor& descriptor) {
            return descriptor.type == type;
        });

    if (it != automationDescriptors.end()) {
        return it->create(name, output);
    }
    spdlog::error("Unknown AutomationType: {}", (int)type);
    return nullptr;
}

}  // namespace automation
