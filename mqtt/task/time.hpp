#pragma once

#include <spdlog/spdlog.h>
#include "../task.hpp"
#include "../observable.hpp"
#include "../event_data.hpp"

namespace task {

class TaskTime : public Observable, public IObservableTask {
public:
    TaskTime() = default;

    void initialize() override {
        execute();
    }

    void execute() override {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&in_time_t);

        hour = local_tm.tm_hour;
        minute = local_tm.tm_min;
        int new_weekday = local_tm.tm_wday;

        sendNotification(hour, minute);
		sendWeekdayNotification(new_weekday);
    }

    bool isValidEvent(EventId eventId) override {
        return (eventId == time.id() || 
                eventId == weekday_event.id());
    }

protected:
    void sendNotification(int hour, int minute) {
        // spdlog::info("Time: {}:{} weekday: {}", hour, minute, weekday);

        int hours = hm2time(hour, minute);
        time.set(hours);
        notify(time);
    }
	void sendWeekdayNotification(int weekday) {
		if (weekday_event.set(weekday)) {
            notify(weekday_event);
		}
	}
    void sendDateNotification(int ayear, int amonth, int aday) {
        if (year.set(ayear)) {
            notify(year);
        }
        if (month.set(amonth)) {
            notify(month);
        }
        if (day.set(aday)) {
            notify(day);
        }

    }
    int hour {0};
    int minute {1};

private:
    EventData time  {EventId::TIME, 0};
    EventData year  {EventId::YEAR, 2025};
    EventData month {EventId::MONTH, 1};
    EventData day   {EventId::DAY, 1};
    EventData weekday_event {EventId::WEEKDAY, 0};
};

//-----------------------------------------------------------------------------
class TaskDebugTime : public TaskTime {
public:
    TaskDebugTime() = default;

    void initialize() override {
        sendNotification(hour, minute);
		sendWeekdayNotification(0);
        sendDateNotification(iyear, imonth, iday);
    }

    void execute() override {
        incrementTime(1);
        int hours = hm2time(hour, minute);
        int darkness {0};

        if (sunrise <= hours && hours <= sunset) {
            darkness = 0;
        } else {
            darkness = 1;
        }

        if (dark.set(darkness)) {
            notify(dark);
        }

        sendNotification(hour, minute);
    }

private:
    void incrementTime(int minutes);
    void incrementDate();

    int sunrise {hm2time(6, 0)};
    int sunset {hm2time(20, 0)};
    int weekday {0};
    int iyear {2026};
    int imonth {2};
    int iday {2};
    EventData dark {EventId::DARK, -1};

};

}  // namespace task
