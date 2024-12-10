#pragma once
#include "automation.hpp"
#include "../task.hpp"

namespace automation {

struct CarHeater : public Automation  {
    CarHeater(std::shared_ptr<IActuator> actuator, std::string_view name, const int& leaveTime) :
        Automation(actuator, name),
        leaveTime(leaveTime)
    {
        initial_value(false);
    }
    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks, 
        std::string_view name,
        std::shared_ptr<IActuator> actuator,
        const int& leaveTime)
    {
        auto ptr = std::make_shared<CarHeater>(actuator, name, leaveTime);
        tasks.subscribe(ETask::TIME, *ptr);
        tasks.subscribe(ETask::TEMPERATURE, *ptr);
        return ptr;
    }
private:
    int calculateDuration(const float temperature) const;
    int leaveTime;
    int offset = 60;
};

}