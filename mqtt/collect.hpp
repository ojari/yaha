#pragma once

#include "events.hpp"
#include "common.hpp"

/** Collects temperature, elprice and time from sources and publish result into observers.*/
class Collect : Observable {
public:

    void feedTemperature(float temperature) {
        if (values.setTemperature(temperature)) {
            notify(values);
        }
    }

    void feedElPrice(int elPrice) {
        if (values.setElPrice(elPrice)) {
            notify(values);
        }
    }

    void feedTime(int hour, int minute) {
        int time = hm2time(hour, minute);
        if (values.setTime(time)) {
            notify(values);
        }
    }
private:
    Values values;
};