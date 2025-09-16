#include <sstream>
#include <iomanip>
#include "common.hpp"


int hm2time(int hour, int minute) {
    return hour * 100 + minute;
}

int dm2date(int day, int month) {
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return -1; // Invalid date
    }
    return day * 100 + month;
}

int dm2day(int date) {
    return date / 100; // Extract day from date (DDMM format)
}

int dm2month(int date) {
    return date % 100; // Extract month from date (DDMM format)
}
int month2mday(int month) {
    if (month == 2) {
        return 28; // February, not considering leap years
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30; // April, June, September, November
    }
    return 31; // Other months
}

int dm2yday(int date) {
    int day = dm2day(date);
    int month = dm2month(date);
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return -1; // Invalid date
    }
    // Calculate the day of the year (yday)
    int yday = 0;
    for (int m = 1; m < month; ++m) {
        yday += month2mday(m);
    }
    return yday + day;
}


std::string time2str(int time) {
    int hour = time / 100;
    int minute = time % 100;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << minute;
    return oss.str();
}

int timeAdd(int time, int minutes) {
    int hour = time / 100;
    int minute = time % 100;
    minute += minutes;
    if (minute >= 60) {
        hour += minute / 60;
        minute %= 60;
    }
    if (minute < 0) {
        hour += (minute - 59) / 60;
        minute = (minute % 60);
        if (minute != 0) {
            minute += 60;
        }
    }
    if (hour >= 24) {
        hour %= 24;
    }
    if (hour < 0) {
        hour += 24;
    }
    return hm2time(hour, minute);
}
