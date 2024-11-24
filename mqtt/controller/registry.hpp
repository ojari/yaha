#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include "automation.hpp"
#include "../data/config.hpp"

namespace controller {

class ControllerRegistry {
public:
    void registerCtrl(
        ITaskManager& tasks,
        const std::string_view& name,
        const std::string& type,
        std::shared_ptr<IActuator> actuator,
        int onTime,
        int offTime,
        int leaveTime)
    {
        auto ctrl = createController(tasks, name, type, actuator, onTime, offTime, leaveTime);
        controllers.push_back(ctrl);
    }
    void registerCtrl(
        ITaskManager& tasks,
        std::shared_ptr<IActuator> actuator,
        const ConfigController& output)
    {
        auto ctrl = createController(
            tasks,
            output.name,
            output.type,
            actuator,
            output.time1,
            output.time2,
            output.time1);
        controllers.push_back(ctrl);
    }

private:
    std::vector<std::shared_ptr<Automation>> controllers;

    std::shared_ptr<Automation> createController(
        ITaskManager& tasks,
        const std::string_view& name,
        const std::string& type,
        std::shared_ptr<IActuator> actuator,
        int onTime,
        int offTime,
        int leaveTime) const;
};
}