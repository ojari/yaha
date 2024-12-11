#pragma once
#include "automation.hpp"
#include "../task.hpp"

namespace automation {

struct CarHeater : public Automation  {
    CarHeater(std::shared_ptr<IActuator> actuator, std::string_view name, const int& leaveTime) :
        Automation(actuator, name),
        leaveTime(leaveTime)
    {
        initial_value(false);
    }

    void registerEvents(IEventManager& evman) override {
        evman.subscribe(EventId::TIME, *this);
        evman.subscribe(EventId::TEMPERATURE, *this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        std::string_view name,
        std::shared_ptr<IActuator> actuator,
        const int& leaveTime)
    {
        return std::make_shared<CarHeater>(actuator, name, leaveTime);
    }
private:
    int calculateDuration(const float temperature) const;
    int leaveTime;
    int offset = 60;
};

}