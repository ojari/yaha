#pragma once
#include <memory>
#include <string>
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

struct Lights : public Automation {
    Lights(std::shared_ptr<IActuator> actuator, const std::string& name) :
        Automation(actuator, name)
    {
        initial_value(false);
    }

    Lights(std::shared_ptr<IActuator> actuator, const std::string& name, int onTime, int offTime) :
        Automation(actuator, name),
        onTime(onTime),
        offTime(offTime)
    {
        initial_value(false);
    }

    void setArg(const std::string& name, const std::string& value) override;

    void registerEvents(IEventManager& evman) override {
        evman.subscribe(EventId::TIME, *this);
    }

    std::string toString() override;

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IActuator> actuator)
    {
        return std::make_shared<Lights>(actuator, name);
    }
private:
    int onTime = -1;
    int offTime = -1;
    int brightness = NAN_VALUE;
};

}