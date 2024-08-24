#pragma once
#include "boolean_controller.hpp"

namespace controller {

class StorageHeater : public BooleanController  {
public:
    explicit StorageHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "StorageHeater")
    {}
    void onChange(const ValueItem& value) override;
private:
    int calculateStartTime(const ValueItem& value);
    int calculateEndTime(const ValueItem& value);
};

}