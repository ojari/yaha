#pragma once
#include "../task.hpp"
#include "../common.hpp"
#include "../demo/suntime.hpp"
// #include "../demo/sun_data.h"
#include <spdlog/spdlog.h>

namespace task {

class TaskSimpleCalcSun {
public:
    TaskSimpleCalcSun(EventBus& aevbus) :
        evbus(aevbus)
    {
        evbus.subscribe<TimeEvent>([&](const TimeEvent& e) {
            bool dark = isDark(e.hour, e.minute);
            if (dark != isDarkLast) {
                isDarkLast = dark;
                evbus.publish(DarkEvent(dark));
            }            
        });
        evbus.subscribe<DateEvent>([&](const DateEvent& e) {
            calculateDark(e.month, e.day);
        });
    }

    bool isDark(int hour, int minute) {
        int currentTime = hm2time(hour, minute);

        if (currentTime < 0) { // Invalid time
            return false;
        }
        if (sunrise == 0 && sunset == 0) {
            return false;
        }
        if (sunrise >= sunset) {
            return false;
        }
        if (currentTime < sunrise || currentTime > sunset) {
            return true;
        }
        return false;
    }
    void calculateDark(int month, int day);

private:
    EventBus& evbus;
    int sunrise = 0;
    int sunset = 0;
    bool isDarkLast = false;
};


class TaskCalcSun : public ITask {
public:
    TaskCalcSun(EventBus& aevbus) :
        evbus(aevbus)
    {
        evbus.subscribe<LocationEvent>([&](const LocationEvent& e) {
            sunTime.setLocation(e.latitude, e.longitude);
        });
        evbus.subscribe<TimeEvent>([&](const TimeEvent& e) {
            time = e.GetTime();
        });
        evbus.subscribe<DateEvent>([&](const DateEvent& e) {
            sunTime.setDate(e.year, e.month, e.day);
        });
    }

    void execute() override {
		int yday = dm2yday(date);

        //int sunriseTime = GetSunrise(yday);
        //int sunsetTime = GetSunset(yday);

            // evbus->publish(sunrise);
            // evbus->publish(sunset);
        /*
        auto sunriseTime = sunTime.calculate(true);
        auto sunsetTime = sunTime.calculate(false);

        if (sunrise.set(sunTime.time2hm(sunriseTime))) {
            evbus->publish(sunrise);
        }
        if(sunset.set(sunTime.time2hm(sunsetTime))) {
            evbus->publish(sunset);
        }

        if (sunrise.getInt() < time && sunset.getInt() > time) {
            if (dark.set(0)) {
                evbus->publish(dark);
            }
        } else {
            if (dark.set(1)) {
                evbus->publish(dark);
            }
        }*/
    }

private:
    EventBus& evbus;

    SunTime sunTime;
    float latitude = 60.2055f;
    float longitude = 24.6559f;
    int time = 0;
    int year = 2026;
	int date = 0101; // Default date in DDMM format
	int day = 1; // Default day
	int month = 1; // Default month
    int last_yday = -1;
};

}  // namespace task
