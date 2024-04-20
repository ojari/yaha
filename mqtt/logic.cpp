#include "logic.hpp"
#include "common.hpp"
#include <iostream>

void DebugOutput::onChange(const Values& vars) {
    std::cout << "Temperature: " << vars.getTemperature() << " ElPrice: " << vars.getElPrice() << " Time: " << vars.getTime() << std::endl;
}

void BooleanOutput::set(bool value) {
    if (value != state) {
        std::cout << name << " changed to " << value << std::endl;
        state = value;
    }
}


void Lights::onChange(const Values& vars) {
    int time = vars.getTime();
    if (time >= onTime && time < offTime) {
        set(true);
    } else {
        set(false);
    }
}

void CarHeater::onChange(const Values& vars) {
    int time = vars.getTime();
    int startTime = timeMinus(leaveTime, calculateDuration(vars));
    if ((time >= startTime) && time < leaveTime) {
        set(true);
    } else {
        set(false);
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
