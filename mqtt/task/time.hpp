#pragma once

#include <spdlog/spdlog.h>
#include "../task.hpp"
#include "../observable.hpp"
#include "../event_data.hpp"

namespace task {

class TaskTime : public Observable, public ITask {
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

        if (new_weekday != weekday) {
            weekday_event.set(new_weekday);
            notify(weekday_event);
            weekday = new_weekday;
        }
    }

protected:
    void sendNotification(int hour, int minute) {
        // spdlog::info("Time: {}:{} weekday: {}", hour, minute, weekday);

        int hours = hm2time(hour, minute);
        time.set(hours);
        notify(time);
    }
    int hour {0};
    int minute {1};
    int weekday {-1};

private:
    EventData time {EventId::TIME, 0};
    EventData weekday_event {EventId::WEEKDAY, 0};
};

//-----------------------------------------------------------------------------
class TaskDebugTime : public TaskTime {
public:
    TaskDebugTime() = default;

    void initialize() override {
        sendNotification(hour, minute);
    }

    void execute() override {
        incrementTime(1);
        int hours = hm2time(hour, minute);
        int sundown {0};

        if (sunrise <= hours && hours <= sunset) {
            sundown = 0;
        } else {
            sundown = 1;
        }

        if (event.set(sundown)) {
            notify(event);
        }

        sendNotification(hour, minute);
    }

private:
    void incrementTime(int minutes);

    int sunrise {hm2time(6, 0)};
    int sunset {hm2time(20, 0)};
    EventData event {EventId::SUNDOWN, 0};

};

}  // namespace task
