#include "car_heater.hpp"

namespace automation {

void CarHeater::onChange(const IValueItem& value) {
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
int CarHeater::calculateDuration(const IValueItem& value) const {
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

}