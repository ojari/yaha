#include <iostream>
#include <iomanip> // Add this line

void showError(const char* message) {
    std::cerr << "ERROR: " << message << std::endl;
}

void showError(const char* message, const char* arg) {
    std::cerr << "ERROR: " << message << " " << arg << std::endl;
}

int hm2time(int hour, int minute) {
    return hour * 100 + minute;
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