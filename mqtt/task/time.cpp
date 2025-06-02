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
            
			incrementDate();
        }
    }
}

void TaskDebugTime::incrementDate() {
    iday++;
    if (iday > 31) { // Simplified, does not account for month lengths
        iday = 1;
        imonth++;
        if (imonth > 12) {
            imonth = 1;
            iyear++;
        }
    }
    sendDateNotification(iyear, imonth, iday);

    weekday = (weekday + 1) % 7;
    sendWeekdayNotification(weekday);
}

}  // namespace task
