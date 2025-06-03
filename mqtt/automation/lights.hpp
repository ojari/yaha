#pragma once
#include <memory>
#include <string>
#include "automation.hpp"

namespace automation {

struct Lights : public Automation {
    Lights(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name)
    {
        initial_value(false);
    }

    Lights(std::shared_ptr<IOutput> output, const std::string& name, int onTime, int offTime) :
        Automation(output, name),
        onTime(onTime),
        offTime(offTime)
    {
        initial_value(false);
    }

    void setArg(const std::string& name, const std::string& value) override;

    void registerEvents(std::shared_ptr<IEventBus> evbus) override {
        evbus->subscribe(EventId::TIME, this);
    }

    std::string toString() override;

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<Lights>(output, name);
    }

private:
    int onTime = -1;
    int offTime = -1;
    int brightness = NAN_VALUE;
};

}  // namespace automation
