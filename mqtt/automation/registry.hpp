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
        ITaskManager& tasks,
        const std::string_view name,
        AutomationType type,
        std::shared_ptr<IActuator> actuator,
        const int& arg1, const int& arg2)
    {
        auto ctrl = create(tasks, name, type, actuator, arg1, arg2);
        controllers.push_back(ctrl);
    }


    void add(
        ITaskManager& tasks,
        std::shared_ptr<IActuator> actuator,
        const ConfigController& output)
    {
        auto ctrl = create(
            tasks,
            output.name,
            toAutomationType(output.type),
            actuator,
            output.time1,
            output.time2);
        controllers.push_back(ctrl);
    }

private:
    std::vector<std::shared_ptr<Automation>> controllers;

    /*template <typename... Args>
    std::shared_ptr<Automation> create(
        ITaskManager& tasks,
        const std::string_view name,
        const std::string& type,
        std::shared_ptr<IActuator> actuator,
        Args&&... args) const;*/

    AutomationType toAutomationType(const std::string& typeStr);

    std::shared_ptr<Automation> create(
        ITaskManager& tasks,
        const std::string_view name,
        AutomationType type,
        std::shared_ptr<IActuator> actuator,
        const int& arg1, const int& arg2) const;

};

}