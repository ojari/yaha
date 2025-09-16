#include "car_heater.hpp"

namespace automation {

void CarHeater::onEvent(const TimeEvent& event) {
    int time = event.GetTime();
    int startTime = timeAdd(leaveTime, -offset);
    if ((time >= startTime) && time < leaveTime) {
        send(true);
    }
    else {
        send(false);
    }
}

void CarHeater::onEvent(const TemperatureEvent& event) {
    offset = calculateDuration(event.value);
}

// Calculate the duration of the car heater based on the temperature
// The duration is calculated in minutes
// The temperature is in degrees Celsius
//
int CarHeater::calculateDuration(float temperature) const {
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

}  // namespace automation
