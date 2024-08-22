#include "logic.hpp"
#include "common.hpp"
#include <iostream>

void DebugOutput::onChange(const ValueItem& value) {
    std::cout << value.name() << " changed to ";
    if (value.isInt()) {
        std::cout << value.getInt();
    } else {
        std::cout << value.getFloat();
    }
    std::cout << std::endl;
}

void BooleanController::set(bool value) {
    if (value != state) {
        actuator->set(name, value);
        if (verbose) {
            std::cout << name << " changed to " << value << std::endl;
        }
        state = value;
    }
}


void Lights::onChange(const ValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        int time = value.getInt();
        if (time >= onTime && time < offTime) {
            set(true);
        } else {
            set(false);
        }
    }
}

void CarHeater::onChange(const ValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        int time = value.getInt();
        int startTime = timeAdd(leaveTime, -offset);
        if ((time >= startTime) && time < leaveTime) {
            set(true);
        } else {
            set(false);
        }
    }
    else if (value.getType() == ValueType::TEMPERATURE) {
        offset = calculateDuration(value);
    }
}

// Calculate the duration of the car heater based on the temperature
// The duration is calculated in minutes
// The temperature is in degrees Celsius
//
int CarHeater::calculateDuration(const ValueItem& value) const {
    float temperature = value.getFloat();
    if (temperature < -20.0f) {
        return 120;
    } else if (temperature < -10.0f) {
        return 90;
    } else if (temperature < -5.0f) {
        return 60;
    } else if (temperature < 5.0f) {
        return 30;
    } else {
        return 0;
    }
}
