#include "actuator.hpp"
#include "events.hpp"
#include <iostream>

void Actuator::onChange(const ValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        time = value.getInt();
    }   
}

void Actuator::set(std::string_view device, bool value) {
    std::cout << time2str(time) << " setting " << device << " to " << (value ? "on" : "off") << std::endl;

    // output->send(device, (value ? "on" : "off"));
}
