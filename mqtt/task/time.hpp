#pragma once

#include "../task.hpp"
#include "../observable.hpp"
#include "../event_data.hpp"

namespace task {

class TaskTime : public Observable, public ITask {
public:
    explicit TaskTime() = default;

    void execute() override {
        incrementTime(1);
        int hours = hm2time(hour, minute);
        if (hours == sunrise) {
            notify(EventData(EventId::SUNRISE, 1));
        }
        else if (hours == sunset) {
            notify(EventData(EventId::SUNSET, 1));
        }
        time.set(hours);
        notify(time);
    }
private:
    void incrementTime(int minutes);

    EventData time {EventId::TIME, 0};
    int hour {0};
    int minute {0};
    int weekday {0};

    int sunrise {hm2time(6, 0)};
    int sunset {hm2time(20, 0)};
};

}