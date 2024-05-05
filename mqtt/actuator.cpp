#include "actuator.hpp"
#include <iostream>

void Actuator::set(std::string &device, bool value) {
    std::cout << "Setting " << device << " to " << (value ? "on" : "off") << std::endl;
}
