#include "boolean_controller.hpp"
#include <iostream>

namespace controller {

void BooleanController::set(bool value) {
    if (value != state) {
        actuator->set(name, value);
        if (verbose) {
            std::cout << name << " changed to " << value << std::endl;
        }
        state = value;
    }
}

}