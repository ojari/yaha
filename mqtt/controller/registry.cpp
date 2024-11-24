#include <memory>
#include "../task.hpp"
#include "registry.hpp"
#include "lights.hpp"
#include "car_heater.hpp"
#include "storage_heater.hpp"
#include "water_heater.hpp"
#include "room_heater.hpp"

namespace controller {

std::shared_ptr<Automation> ControllerRegistry::createController(
    ITaskManager& tasks,
    const std::string_view& name,
    const std::string& type,
    std::shared_ptr<IActuator> actuator,
    int onTime,
    int offTime,
    int leaveTime) const
{
    if (type == "CarHeater") {
        return CarHeater::create(tasks, actuator, leaveTime);
    }
    else if (type == "Lights") {
        return Lights::create(tasks, name, actuator, onTime, offTime);
    }
    else if (type == "StorageHeater") {
        return StorageHeater::create(tasks, actuator);
    }
    else if (type == "WaterHeater") {
        return WaterHeater::create(tasks, actuator);
    }
    else if (type == "RoomHeater") {
        return RoomHeater::create(tasks, actuator);
    }

    // Handle unknown type
    return nullptr;
}

}