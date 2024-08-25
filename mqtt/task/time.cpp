#include "time.hpp"

namespace task {

void TaskTime::incrementTime(int minutes) {
    minute += minutes;
    if (minute >= 60) {
        hour += minute / 60;
        minute %= 60;
        if (hour >= 24) {
            hour %= 24;
            weekday = (weekday + 1) % 7;
            notify(ValueItem(ValueType::WEEKDAY, weekday));
        }
    }
}

}