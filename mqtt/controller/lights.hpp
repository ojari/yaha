#pragma once
#include <memory>
#include <string>
#include "automation.hpp"
#include "../task_manager.hpp"

namespace controller {

struct Lights : public Automation {
    Lights(std::shared_ptr<IActuator> actuator, std::string_view name, int onTime, int offTime) :
        Automation(actuator, name),
        onTime(onTime),
        offTime(offTime)
    {}

    void onChange(const IValueItem& value) override;

    static std::shared_ptr<Automation> create(
        ITaskManager& tasks,
        std::string_view name,
        std::shared_ptr<IActuator> actuator,
        int onTime,
        int offTime)
    {
        auto ptr = std::make_shared<Lights>(actuator, name, onTime, offTime);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
private:
    int onTime;
    int offTime;
};

}