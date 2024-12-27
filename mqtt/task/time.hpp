#pragma once

#include "../task.hpp"
#include "../observable.hpp"
#include "../event_data.hpp"
#include <spdlog/spdlog.h>

namespace task {

class TaskTime : public Observable, public ITask {
public:
    explicit TaskTime() = default;

    void execute() override {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&in_time_t);

        hour = local_tm.tm_hour;
        minute = local_tm.tm_min;
        weekday = local_tm.tm_wday;

        sendNotification(hour, minute);
    }

protected:
    void sendNotification(int hour, int minute) {
        // spdlog::info("Time: {}:{} weekday: {}", hour, minute, weekday);

        int hours = hm2time(hour, minute);
        time.set(hours);
        notify(time);
    }
    int hour {0};
    int minute {0};
    int weekday {0};

private:
    EventData time {EventId::TIME, 0};
};

//-----------------------------------------------------------------------------
class TaskDebugTime : public TaskTime {
public:
    explicit TaskDebugTime() = default;

    void execute() override {
        incrementTime(1);
        int hours = hm2time(hour, minute);
        if (hours == sunrise) {
            notify(EventData(EventId::SUNRISE, 1));
        }
        else if (hours == sunset) {
            notify(EventData(EventId::SUNSET, 1));
        }

        sendNotification(hour, minute);
    }
private:
    void incrementTime(int minutes);

    int sunrise {hm2time(6, 0)};
    int sunset {hm2time(20, 0)};
};

}