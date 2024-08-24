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
private:
    int onTime;
    int offTime;
};

}