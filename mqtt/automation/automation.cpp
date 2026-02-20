#include "automation.hpp"
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
        output->sendCommand(name, commandValues());
        if (verbose) {
            spdlog::info("{} changed to {}", name, value);
        }
    }
}

void Automation::send(bool value) {
    if (value != std::get<bool>(state)) {
        state = value;
        output->sendCommand(name, commandValues());
        if (verbose) {
            spdlog::info("{} changed to {}", name, value);
        }
    }
}

}  // namespace automation
