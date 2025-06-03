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

    void registerEvents(std::shared_ptr<IEventBus> evbus) override {
        evbus->subscribe(EventId::TIME, this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<RoomHeater>(output, name);
    }
};

}  // namespace automation
