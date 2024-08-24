#pragma once
#include "boolean_controller.hpp"

namespace controller {

struct CarHeater : public BooleanController  {
    CarHeater(std::shared_ptr<IActuator> actuator, int leaveTime) :
        BooleanController(actuator, "CarHeater"),
        leaveTime(leaveTime)
    {}
    void onChange(const ValueItem& value) override;
private:
    int calculateDuration(const ValueItem& value) const;
    int leaveTime;
    int offset = 60;
    bool state;
};

}