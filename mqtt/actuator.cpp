#include "actuator.hpp"
#include <spdlog/spdlog.h>

void Actuator::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        time = event.getInt();
    }   
}

void Actuator::set(std::string_view device, bool value) {
    spdlog::info("{}: setting {} to {}",time2str(time), device, (value ? "on" : "off"));

    // output->send(device, (value ? "on" : "off"));
}
