#pragma once
#include <memory>
#include <string>
#include "boolean_controller.hpp"

namespace controller {

struct Lights : public BooleanController  {    
    Lights(std::shared_ptr<IActuator> actuator, int onTime, int offTime) :
        BooleanController(actuator, "Lights"),
        onTime(onTime),
        offTime(offTime)
    {}

    void onChange(const ValueItem& value) override;

    static std::shared_ptr<BooleanController> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator,
        int onTime,
        int offTime)
    {
        auto ptr = std::make_shared<Lights>(actuator, onTime, offTime);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
private:
    int onTime;
    int offTime;
};

}