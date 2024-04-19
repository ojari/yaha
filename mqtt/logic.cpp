#include "logic.hpp"
#include <iostream>

void DebugOutput::onChange(const Values& state) {
    std::cout << "Temperature: " << state.getTemperature() << " ElPrice: " << state.getElPrice() << " Time: " << state.getTime() << std::endl;
}