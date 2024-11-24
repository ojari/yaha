#pragma once
#include "automation.hpp"
#include "../task_manager.hpp"

namespace controller {

class WaterHeater : public Automation  {
public:
    explicit WaterHeater(std::shared_ptr<IActuator> actuator) :
        Automation(actuator, "WaterHeater")
    {}

    void onChange(const IValueItem& value) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<WaterHeater>(actuator);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
};

}