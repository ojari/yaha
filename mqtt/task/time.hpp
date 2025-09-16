#pragma once

#include <spdlog/spdlog.h>
#include "../task.hpp"

namespace task {

class TaskTime : public ITask {
public:
    TaskTime(EventBus& evbus) :
        evbus(evbus)
    {
    }

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
		// sendWeekdayNotification(new_weekday);
    }

protected:
    void sendNotification(int hour, int minute) {
        // spdlog::info("Time: {}:{} weekday: {}", hour, minute, weekday);

        evbus.publish<TimeEvent>(TimeEvent(hour, minute));
    }

    
    void sendDateNotification(int ayear, int amonth, int aday) {
        evbus.publish<DateEvent>(DateEvent(ayear, amonth, aday, 0));
    }
    int hour {0};
    int minute {1};
    EventBus& evbus;
};

//-----------------------------------------------------------------------------
class TaskDebugTime : public TaskTime {
public:
    TaskDebugTime(EventBus& evbus) :
        TaskTime(evbus)
    {
    }

    void initialize() override {
        sendNotification(hour, minute);
		// sendWeekdayNotification(0);
        sendDateNotification(iyear, imonth, iday);
    }

    void execute() override {
        incrementTime(5);
        int hours = hm2time(hour, minute);
        int darkness {0};

        if (sunrise <= hours && hours <= sunset) {
            darkness = 0;
        } else {
            darkness = 1;
        }

        evbus.publish<DarkEvent>(DarkEvent(darkness));

        sendNotification(hour, minute);
    }

private:
    void incrementTime(int minutes);
    void incrementDate();

    int sunrise {hm2time(6, 0)};
    int sunset {hm2time(20, 0)};
    int weekday {0};
    int iyear {2026};
    int imonth {4};
    int iday {15};

};

}  // namespace task
