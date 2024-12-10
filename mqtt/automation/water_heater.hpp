#pragma once
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

class WaterHeater : public Automation  {
public:
    explicit WaterHeater(std::shared_ptr<IActuator> actuator, std::string_view name) :
        Automation(actuator, name)
    {
        initial_value(false);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks,
        std::string_view name,
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<WaterHeater>(actuator, name);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
};

}