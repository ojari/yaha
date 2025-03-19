#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include "automation.hpp"
#include "../task_manager.hpp"
#include "../data/config.hpp"

namespace automation {

class Registry {
public:
    explicit Registry(std::shared_ptr<IActuator> actuator) :
        actuator { actuator }
    {}

    void load(const std::string& filename, IEventManager& evman);

    /*template <typename... Args>
    void add(
        ITaskManager& tasks,
        const std::string_view name,
        const std::string& type,
        std::shared_ptr<IActuator> actuator,
        Args&&... args)
    {
        auto ctrl = create(tasks, name, type, actuator, std::forward<Args>(args)...);
        controllers.push_back(ctrl);
    }
    */
    void add(
        IEventManager& evman,
        const std::string& name,
        AutomationType type)
    {
        auto ctrl = create(name, type);
        ctrl->registerEvents(evman);
        controllers.push_back(ctrl);
    }

private:
    std::vector<std::shared_ptr<Automation>> controllers;
    std::shared_ptr<IActuator> actuator;

    /*template <typename... Args>
    std::shared_ptr<Automation> create(
        ITaskManager& tasks,
        const std::string_view name,
        const std::string& type,
        std::shared_ptr<IActuator> actuator,
        Args&&... args) const;*/

    AutomationType toAutomationType(const std::string& typeStr);

    std::shared_ptr<Automation> create(
        const std::string& name,
        AutomationType type) const;
};

}  // namespace automation
