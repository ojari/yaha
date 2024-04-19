#include "logic.hpp"
#include <iostream>

void DebugOutput::onChange(const Values& state) {
    std::cout << "Temperature: " << state.getTemperature() << " ElPrice: " << state.getElPrice() << " Time: " << state.getTime() << std::endl;
}

void Lights::onChange(const Values& state) {
    int time = state.getTime();
    if (time >= onTime && time < offTime) {
        std::cout << "Lights are on" << std::endl;
    } else {
        std::cout << "Lights are off" << std::endl;
    }
}

void CarHeater::onChange(const Values& state) {
    int time = state.getTime();
    //@todo: implement time addition
    if ((time >= leaveTime - calculateDuration(state)) && time < leaveTime) {
        std::cout << "Car heater is on" << std::endl;
    } else {
        std::cout << "Car heater is off" << std::endl;
    }
}

int CarHeater::calculateDuration(const Values& state) {
    int temperature = state.getTemperature();
    if (temperature < -20) {
        return 120;
    } else if (temperature < -10) {
        return 90;
    } else if (temperature < -5) {
        return 60;
    } else if (temperature < 5) {
        return 30;
    } else {
        return 0;
    }
}
