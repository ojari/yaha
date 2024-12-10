#pragma once
#include "automation.hpp"
#include "../task.hpp"

namespace automation {

class StorageHeater : public Automation {
public:
    explicit StorageHeater(std::shared_ptr<IActuator> actuator, std::string_view name) :
        Automation(actuator, name)
    {
        initial_value(false);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks, 
        std::string_view name,
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<StorageHeater>(actuator, name);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }

private:
    int calculateStartTime(const IEventData& value);
    int calculateEndTime(const IEventData& value);
};

}