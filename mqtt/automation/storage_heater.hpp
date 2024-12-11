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

    void registerEvents(IEventManager& evman) override {
        evman.subscribe(EventId::TIME, *this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        std::string_view name,
        std::shared_ptr<IActuator> actuator)
    {
        return std::make_shared<StorageHeater>(actuator, name);
    }

private:
    int calculateStartTime(const IEventData& value);
    int calculateEndTime(const IEventData& value);
};

}