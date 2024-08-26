#pragma once
#include "boolean_controller.hpp"

namespace controller {

class StorageHeater : public BooleanController  {
public:
    explicit StorageHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "StorageHeater")
    {}

    void onChange(const ValueItem& value) override;

    static std::shared_ptr<BooleanController> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<StorageHeater>(actuator);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }

private:
    int calculateStartTime(const ValueItem& value);
    int calculateEndTime(const ValueItem& value);
};

}