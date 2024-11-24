#pragma once
#include "automation.hpp"
#include "../task_manager.hpp"

namespace controller {

class RoomHeater : public Automation  {
public:
    explicit RoomHeater(std::shared_ptr<IActuator> actuator) :
        Automation(actuator, "RoomHeater")
    {}

    void onChange(const IValueItem& value) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<RoomHeater>(actuator);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
};

}