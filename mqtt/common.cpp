#include <sstream>
#include <iomanip> // Add this line
#include "common.hpp"

struct EventIdString {
    EventId type;
    const char* str;
};

EventIdString eventNames[] = {
    { EventId::TEMPERATURE,          "Temperature" },
    { EventId::TEMPERATURE_ROOM,     "Temperature Room" },
    { EventId::ELECTRICITY_PRICE,    "Electricity Price" },
    { EventId::TIME,                 "Time" },
    { EventId::WEEKDAY,              "Weekday" },
    { EventId::SUNRISE,              "Sunrise" },
    { EventId::SUNSET,               "Sunset" },
    { EventId::BUTTON_LIVING_ROOM,   "Button Living Room" },
    { EventId::BUTTON_LIBRARY,       "Button Library" },
    { EventId::SWITCH_AUTO_TALLI,    "Switch Auto Talli" },
    { EventId::SWITCH_AUTO_ULKO,     "Switch Auto Ulko" },
    { EventId::UNKNOWN,              "Unknown" }
};

std::string event2str(EventId type) {
    for (auto& eventName : eventNames) {
        if (eventName.type == type) {
            return eventName.str;
        }
    }
    return "Unknown";
}

EventId str2event(const std::string& str) {
    for (auto& eventName : eventNames) {
        if (eventName.str == str) {
            return eventName.type;
        }
    }
    return EventId::UNKNOWN;
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