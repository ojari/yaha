#pragma once
#include "automation.hpp"
#include "../task_manager.hpp"

namespace automation {

class WaterHeater : public Automation  {
public:
    explicit WaterHeater(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name)
    {
        initial_value(false);
    }

    void registerEvents(IEventManager& evman) override {
        evman.subscribe(EventId::TIME, *this);
    }

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<WaterHeater>(output, name);
    }
};

}  // namespace automation
