#pragma once
#include "automation.hpp"

namespace automation {

class RoomHeater : public Automation  {
public:
    explicit RoomHeater(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name)
    {
        initial_value(false);
    }

    void registerEvents(EventBus& evbus) override {
        evbus.subscribe<TimeEvent>([&](const TimeEvent& e) {
            // this->onEvent(e);
        });
    }

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<RoomHeater>(output, name);
    }
};

}  // namespace automation
