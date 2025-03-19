#pragma once
#include <memory>
#include <string>
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

struct SwitchLight : public Automation  {
    SwitchLight(std::shared_ptr<IActuator> actuator, const std::string& name) :
        Automation(actuator, name)
    {
        initial_value(0);
    }

    void setArg(const std::string& name, const std::string& value) override;

    std::string toString() override;

    void registerEvents(IEventManager& evman) override {
        evman.subscribe(buttonEvent, *this);
        evman.subscribe(lampEvent, *this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IActuator> actuator)
    {
        return std::make_shared<SwitchLight>(actuator, name);
    }

private:
    void toggleLight();

    EventId buttonEvent = EventId::UNKNOWN;
    EventId lampEvent = EventId::UNKNOWN;
    int mode = 0;
    int brightness = NAN_VALUE;
    bool current = false;
};

}  // namespace automation
