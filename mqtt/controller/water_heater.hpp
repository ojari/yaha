#pragma once
#include "boolean_controller.hpp"

namespace controller {

class WaterHeater : public BooleanController  {
public:
    explicit WaterHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "WaterHeater")
    {}

    void onChange(const ValueItem& value) override;

    static std::shared_ptr<BooleanController> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<WaterHeater>(actuator);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
};

}