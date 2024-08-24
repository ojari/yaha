#pragma once
#include "boolean_controller.hpp"

namespace controller {

class RoomHeater : public BooleanController  {
public:
    explicit RoomHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "RoomHeater")
    {}
    void onChange(const ValueItem& value) override;
};

}