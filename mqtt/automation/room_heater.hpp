#pragma once
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

class RoomHeater : public Automation  {
public:
    explicit RoomHeater(std::shared_ptr<IActuator> actuator, std::string_view name) :
        Automation(actuator, name)
    {
        initial_value(false);
    }

    void onChange(const IValueItem& value) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks, 
        std::string_view name,
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<RoomHeater>(actuator, name);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
};

}