#pragma once
#include "automation.hpp"
#include "../task.hpp"

namespace automation {

struct CarHeater : public Automation  {
    CarHeater(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name),
		leaveTime(0700)
    {
        initial_value(false);
    }

    CarHeater(std::shared_ptr<IOutput> output, const std::string& name, const int& leaveTime) :
        Automation(output, name),
        leaveTime(leaveTime)
    {
        initial_value(false);
    }

    void setArg(const std::string& name, const std::string& value) override {
        if (name == "leave") {
            leaveTime = std::stoi(value);
        } else {
            // throw std::runtime_error("Unknown argument: " + name);
        }
    }

    void registerEvents(std::shared_ptr<IEventBus> evbus) override {
        evbus->subscribe(EventId::TIME, this);
        evbus->subscribe(EventId::TEMPERATURE, this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<CarHeater>(output, name);
    }

private:
    int calculateDuration(const float temperature) const;
    int leaveTime;
    int offset = 60;
};

}  // namespace automation
