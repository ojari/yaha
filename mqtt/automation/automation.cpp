#include "automation.hpp"
#include <iostream>

namespace automation {

void Automation::initial_value(int value) {
    state = value;
}

void Automation::initial_value(bool value) {
    state = value;
}


void Automation::set(int value) {
    if (value != std::get<int>(state)) {
        actuator->set(name, value);
        if (verbose) {
            std::cout << name << " changed to " << value << std::endl;
        }
        state = value;
    }
}

void Automation::set(bool value) {
    if (value != std::get<bool>(state)) {
        actuator->set(name, value);
        if (verbose) {
            std::cout << name << " changed to " << value << std::endl;
        }
        state = value;
    }
}

}