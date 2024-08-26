#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include "boolean_controller.hpp"

namespace controller {

class ControllerRegistry {
public:
    void registerCtrl(std::shared_ptr<BooleanController> output) {
        controllers.push_back(output);
    }

private:
    std::vector<std::shared_ptr<BooleanController>> controllers;

    std::shared_ptr<BooleanController> createController(
        ITaskManager& tasks,
        const std::string& type,
        std::shared_ptr<IActuator> actuator,
        int onTime,
        int offTime,
        int leaveTime) const;
};
}