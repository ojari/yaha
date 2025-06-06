#include "automation.hpp"
#include "../config.hpp"
#include <spdlog/spdlog.h>

namespace automation {

void Automation::initial_value(int value) {
    state = value;
}

void Automation::initial_value(bool value) {
    state = value;
}

void Automation::send(int value) {
    if (value != std::get<int>(state)) {
        state = value;
        std::string topic = std::string(MQTT_TOPIC) + "/" + name + "/set";
        output->send(topic, toString());
        if (verbose) {
            spdlog::info("{} changed to {}", name, value);
        }
    }
}

void Automation::send(bool value) {
    if (value != std::get<bool>(state)) {
        state = value;
        std::string topic = std::string(MQTT_TOPIC) + "/" + name + "/set";
        output->send(topic, toString());
        if (verbose) {
            spdlog::info("{} changed to {}", name, value);
        }
    }
}

}  // namespace automation
