#pragma once
#include <memory>
#include <string>
#include "automation.hpp"

namespace automation {

struct SwitchLight : public Automation {
    SwitchLight(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name)
    {
        initial_value(0);
    }

    void setArg(const std::string& name, const std::string& value) override;

    std::string toString() override;

    void registerEvents(std::shared_ptr<IEventBus> evbus) override {
        evbus->subscribe(buttonEvent, this);
        evbus->subscribe(lampEvent, this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<SwitchLight>(output, name);
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
