#include <memory>
#include "registry.hpp"
#include "boolean_controller.hpp"
#include "lights.hpp"
#include "car_heater.hpp"
#include "storage_heater.hpp"
#include "water_heater.hpp"
#include "room_heater.hpp"

namespace controller {

std::shared_ptr<BooleanController> ControllerRegistry::createController(
    const std::string& type,
    std::shared_ptr<IActuator> actuator,
    int onTime,
    int offTime,
    int leaveTime) const
{
    std::unordered_map<std::string, std::function<std::shared_ptr<BooleanController>()>> creators = {
        {"Lights", [actuator, onTime, offTime]() {
            return std::make_shared<Lights>(actuator, onTime, offTime);
        }},
        {"CarHeater", [actuator, leaveTime]() {
            return std::make_shared<CarHeater>(actuator, leaveTime);
        }},
        {"StorageHeater", [actuator]() {
            return std::make_shared<StorageHeater>(actuator);
        }},
        {"WaterHeater", [actuator]() {
            return std::make_shared<WaterHeater>(actuator);
        }},
        {"RoomHeater", [actuator]() {
            return std::make_shared<RoomHeater>(actuator);
        }}
    };

    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second();
    } else {
        // Handle unknown type
        return nullptr;
    }
}

}