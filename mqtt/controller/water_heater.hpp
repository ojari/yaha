#pragma once
#include "boolean_controller.hpp"

namespace controller {

class WaterHeater : public BooleanController  {
public:
    explicit WaterHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "WaterHeater")
    {}
    void onChange(const ValueItem& value) override;
};

}