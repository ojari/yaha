#pragma once
#include <string>
#include <memory>

namespace data {

struct ConfigDevice {
    std::string name;
    std::string type;

    ConfigDevice() = default;

    ConfigDevice(const std::string& name, const std::string& type) :
        name(name),
        type(type)
    {}
};

struct ConfigController {
    std::string name;
    std::string type;
    std::string actuator;
    int time1;
    int time2;

    ConfigController() = default;

    ConfigController(const std::string& name,
                     const std::string& type,
                     const std::string& actuator,
                     int time1,
                     int time2) :
        name(name),
        type(type),
        actuator(actuator),
        time1(time1),
        time2(time2)
    {}
};

}  // namespace data
