#include "automation.hpp"
#include <spdlog/spdlog.h>

namespace automation {

void Automation::initial_value(int value) {
    state = value;
}

void Automation::initial_value(bool value) {
    state = value;
}

void Automation::set(int value) {
    if (value != std::get<int>(state)) {
        state = value;
        std::string topic = "zigbee2mqtt/" + name + "/set";
        actuator->set(topic, toString());
        if (verbose) {
            spdlog::info("{} changed to {}", name, value);
        }
    }
}

void Automation::set(bool value) {
    if (value != std::get<bool>(state)) {
        state = value;
        std::string topic = "zigbee2mqtt/" + name + "/set";
        actuator->set(topic, toString());
        if (verbose) {
            spdlog::info("{} changed to {}", name, value);
        }
    }
}

}  // namespace automation
