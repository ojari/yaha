#pragma once
#include <memory>
#include <string>
#include "base_controller.hpp"
#include "../task_manager.hpp"

namespace controller {

struct SwitchLights : public BaseController<int>  {    
    SwitchLights(std::shared_ptr<IActuator> actuator, std::string_view name) :
        BaseController(actuator, name)
    {}

    void onChange(const IValueItem& value) override;

    static std::shared_ptr<BaseController> create(
        ITaskManager& tasks,
        std::string_view name,
        std::shared_ptr<IActuator> actuator)
    {
        auto ptr = std::make_shared<SwitchLights>(actuator, name);
        tasks.subscribe(ETask::TIME, *ptr);
        return ptr;
    }
private:
};

}