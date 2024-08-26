#pragma once
#include "boolean_controller.hpp"
#include "../events.hpp"

namespace controller {

struct CarHeater : public BooleanController  {
    CarHeater(std::shared_ptr<IActuator> actuator, int leaveTime) :
        BooleanController(actuator, "CarHeater"),
        leaveTime(leaveTime)
    {}
    void onChange(const ValueItem& value) override;

    static std::shared_ptr<BooleanController> create(
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
    int calculateDuration(const ValueItem& value) const;
    int leaveTime;
    int offset = 60;
    bool state;
};

}