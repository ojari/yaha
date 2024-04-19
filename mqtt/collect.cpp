#include "collect.hpp"


void SourceTime::incrementTime(int minutes) {
    minute += minutes;
    if (minute >= 60) {
        hour += minute / 60;
        minute %= 60;
        if (hour >= 24) {
            hour %= 24;
        }
    }
}
