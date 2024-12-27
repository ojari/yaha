#include "time.hpp"
#include "../common.hpp"

namespace task {

void TaskDebugTime::incrementTime(int minutes) {
    minute += minutes;
    if (minute >= 60) {
        hour += minute / 60;
        minute %= 60;
        if (hour >= 24) {
            hour %= 24;
            weekday = (weekday + 1) % 7;
            notify(EventData(EventId::WEEKDAY, weekday));
        }
    }
}

}