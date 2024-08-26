#pragma once
#include "boolean_controller.hpp"

namespace controller {

class RoomHeater : public BooleanController  {
public:
    explicit RoomHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "RoomHeater")
    {}

    void onChange(const ValueItem& value) override;

    static std::shared_ptr<BooleanController> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<RoomHeater>(actuator);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
};

}