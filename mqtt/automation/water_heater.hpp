#pragma once
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

class WaterHeater : public Automation  {
public:
    explicit WaterHeater(std::shared_ptr<IActuator> actuator, const std::string& name) :
        Automation(actuator, name)
    {
        initial_value(false);
    }

    void registerEvents(IEventManager& evman) override {
        evman.subscribe(EventId::TIME, *this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IActuator> actuator)
    {
        return std::make_shared<WaterHeater>(actuator, name);
    }
};

}