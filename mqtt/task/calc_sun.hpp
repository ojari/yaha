#pragma once
#include "../task.hpp"
#include "../common.hpp"
#include "../event_data.hpp"
#include "../demo/suntime.hpp"
#include "../demo/sun_data.h"
#include <spdlog/spdlog.h>

namespace task {

class TaskCalcSun : public ITask, public IObserver {
public:
    TaskCalcSun(std::shared_ptr<IEventBus> aevbus) :
        evbus(aevbus)
    {
        evbus->subscribe(EventId::LATITUDE, this);
        evbus->subscribe(EventId::LONGITUDE, this);
        evbus->subscribe(EventId::TIME, this);
        evbus->subscribe(EventId::YEAR, this);
        evbus->subscribe(EventId::DATE, this);
    }

    void onChange(const IEventData& value) override {
        switch (value.id()) {
            case EventId::LATITUDE:
                latitude = value.getFloat();
                spdlog::info("{} latitude={}  longitude={}", time2str(time), latitude, longitude);

                sunTime.setLocation(latitude, longitude);
                break;
            case EventId::LONGITUDE:
                longitude = value.getFloat();
                spdlog::info("{} latitude={}  longitude={}", time2str(time), latitude, longitude);
                sunTime.setLocation(latitude, longitude);
                break;
            case EventId::TIME:
                time = value.getInt();
                break;
            case EventId::YEAR:
                year = value.getInt();
                // spdlog::info("{} year={}  {}", time2str(time), year, date);
                sunTime.setDate(year, month, day);
                break;
            case EventId::DATE:
				date = value.getInt();
                // spdlog::info("{} year={}  {}", time2str(time), year, date);
                sunTime.setDate(year, month, day);
                break;
            default:
                return; // Ignore other events
        }
    }

    void execute() override {
		int yday = dm2yday(date);

        int sunriseTime = GetSunrise(yday);
        int sunsetTime = GetSunset(yday);

        if (sunrise.set(sunriseTime)) {
            evbus->publish(sunrise);
        }
        if (sunset.set(sunsetTime)) {
            evbus->publish(sunset);
        }
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
    std::shared_ptr<IEventBus> evbus;
    EventData sunrise {EventId::SUNRISE, 0600};
    EventData sunset {EventId::SUNSET, 2100};
    EventData dark {EventId::DARK, 0};

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
