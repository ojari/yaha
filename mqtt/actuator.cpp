#include "actuator.hpp"
#include <spdlog/spdlog.h>

void Actuator::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        time = event.getInt();
    }   
}

void Actuator::set(std::string_view device, const std::string& value) {
    spdlog::info("{}: setting {} to {}",time2str(time), device, value);

    output->send(device, value);
}
