#pragma once
#include "automation.hpp"
#include "../task.hpp"

namespace controller {

class StorageHeater : public Automation {
public:
    explicit StorageHeater(std::shared_ptr<IActuator> actuator) :
        Automation(actuator, "StorageHeater")
    {}

    void onChange(const IValueItem& value) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks, 
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<StorageHeater>(actuator);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }

private:
    int calculateStartTime(const IValueItem& value);
    int calculateEndTime(const IValueItem& value);
};

}