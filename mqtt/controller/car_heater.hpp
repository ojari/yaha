#pragma once
#include "automation.hpp"
#include "../task.hpp"

namespace controller {

struct CarHeater : public Automation  {
    CarHeater(std::shared_ptr<IActuator> actuator, int leaveTime) :
        Automation(actuator, "CarHeater"),
        leaveTime(leaveTime)
    {}
    void onChange(const IValueItem& value) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator,
        int leaveTime)
    {
        auto ptr = std::make_shared<CarHeater>(actuator, leaveTime);
        tasks.subscribe(ETask::TIME, *ptr);
        tasks.subscribe(ETask::TEMPERATURE, *ptr);
        return ptr;
    }
private:
    int calculateDuration(const IValueItem& value) const;
    int leaveTime;
    int offset = 60;
    bool state;
};

}