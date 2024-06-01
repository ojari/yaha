#include "logic.hpp"
#include "common.hpp"
#include <iostream>

void DebugOutput::onChange(const IValues& state) {
    const Values& vars = dynamic_cast<const Values&>(state);
    std::cout << vars;
    /*std::cout << "Temperature: " << vars.getFloat(ValueType::TEMPERATURE) 
              << " ElPrice: " << vars.getFloat(ValueType::ELECTRICITY_PRICE)
              << " Time: " << vars.getInt(ValueType::TIME)
              << std::endl;*/
}

void BooleanController::set(bool value) {
    if (value != state) {
        actuator->set(name, value);
        // std::cout << name << " changed to " << value << std::endl;
        state = value;
    }
}


void Lights::onChange(const IValues& vars) {
    int time = vars.getInt(ValueType::TIME);
    if (time >= onTime && time < offTime) {
        set(true);
    } else {
        set(false);
    }
}

void CarHeater::onChange(const IValues& vars) {
    int time = vars.getInt(ValueType::TIME);
    int startTime = timeAdd(leaveTime, -calculateDuration(vars));
    if ((time >= startTime) && time < leaveTime) {
        set(true);
    } else {
        set(false);
    }
}

int CarHeater::calculateDuration(const IValues& state) {
    float temperature = state.getFloat(ValueType::TEMPERATURE);
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
