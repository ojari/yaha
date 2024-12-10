#include "actuator.hpp"
#include <iostream>

void Actuator::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        time = event.getInt();
    }   
}

void Actuator::set(std::string_view device, bool value) {
    std::cout << time2str(time) << " setting " << device << " to " << (value ? "on" : "off") << std::endl;

    // output->send(device, (value ? "on" : "off"));
}
