#include "collect.hpp"


void Collect::feedTemperature(float temperature) {
    if (values.setTemperature(temperature)) {
        notify(values);
    }
}

void Collect::feedElPrice(int elPrice) {
    if (values.setElPrice(elPrice)) {
        notify(values);
    }
}

void Collect::feedTime(int hour, int minute) {
    int time = hm2time(hour, minute);
    if (values.setTime(time)) {
        notify(values);
    }
}


void SourceTime::incrementTime(int minutes) {
    minute += minutes;
    if (minute >= 60) {
        hour += minute / 60;
        minute %= 60;
        if (hour >= 24) {
            hour %= 24;
        }
    }
}
