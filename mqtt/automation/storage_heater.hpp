#pragma once
#include "automation.hpp"
#include "../task.hpp"
#include "../common.hpp"

namespace automation {

class StorageHeater : public Automation {
public:
    explicit StorageHeater(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name)
    {
        initial_value(false);
    }

    void registerEvents(EventBus& evbus) override {
        evbus.subscribe<TimeEvent>([&](const TimeEvent& e) {
            this->onEvent(e);
        });
    }

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<StorageHeater>(output, name);
    }

private:
    void onEvent(const TimeEvent& event);

    //int calculateStartTime(const IEventData& value);
    //int calculateEndTime(const IEventData& value);
};
}