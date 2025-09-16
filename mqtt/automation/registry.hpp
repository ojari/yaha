#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include "automation.hpp"
#include "../data/config.hpp"

namespace automation {

class Registry {
public:
    explicit Registry(std::shared_ptr<IOutput> output, EventBus& evbus) :
		output{ output },
        eventBus{ evbus }
    {}

    void load(const std::string& filename, EventBus& evbus);

    void add(
        EventBus& evbus,
        const std::string& name,
        AutomationType type)
    {
        auto ctrl = create(name, type);
        ctrl->registerEvents(evbus);
        controllers.push_back(ctrl);
    }

private:
    std::vector<std::shared_ptr<Automation>> controllers;
	EventBus& eventBus;
    std::shared_ptr<IOutput> output;

    AutomationType toAutomationType(const std::string& typeStr);

    std::shared_ptr<Automation> create(
        const std::string& name,
        AutomationType type) const;
};

}  // namespace automation
