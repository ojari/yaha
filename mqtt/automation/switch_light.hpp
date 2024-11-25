#pragma once
#include <memory>
#include <string>
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

struct SwitchLights : public Automation  {    
    SwitchLights(std::shared_ptr<IActuator> actuator, std::string_view name) :
        Automation(actuator, name)
    {
        initial_value(0);
    }

    void onChange(const IValueItem& value) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks,
        std::string_view name,
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<SwitchLights>(actuator, name);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
private:
};

}