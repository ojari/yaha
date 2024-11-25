#include <memory>
#include "../task.hpp"
#include "registry.hpp"
#include "lights.hpp"
#include "car_heater.hpp"
#include "storage_heater.hpp"
#include "water_heater.hpp"
#include "room_heater.hpp"

namespace automation {

std::shared_ptr<Automation> Registry::create(
    ITaskManager& tasks,
    const std::string_view name,
    const std::string& type,
    std::shared_ptr<IActuator> actuator,
    const int& arg1,
    const int& arg2) const
{
    if (type == "CarHeater") {
        return CarHeater::create(tasks, name, actuator, arg1);
    }
    else if (type == "Lights") {
        return Lights::create(tasks, name, actuator, arg1, arg2);
    }
    else if (type == "StorageHeater") {
        return StorageHeater::create(tasks, name, actuator);
    }
    else if (type == "WaterHeater") {
        return WaterHeater::create(tasks, name, actuator);
    }
    else if (type == "RoomHeater") {
        return RoomHeater::create(tasks, name, actuator);
    }

    // Handle unknown type
    return nullptr;
}

}