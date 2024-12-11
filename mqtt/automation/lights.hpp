#pragma once
#include <memory>
#include <string>
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

struct Lights : public Automation {
    Lights(std::shared_ptr<IActuator> actuator, std::string_view name, const int& onTime, const int& offTime) :
        Automation(actuator, name),
        onTime(onTime),
        offTime(offTime)
    {
        initial_value(false);
    }

    void registerEvents(IEventManager& evman) override {
        evman.subscribe(EventId::TIME, *this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        std::string_view name,
        std::shared_ptr<IActuator> actuator,
        const int& onTime,
        const int& offTime)
    {
        return std::make_shared<Lights>(actuator, name, onTime, offTime);
    }
private:
    int onTime;
    int offTime;
};

}