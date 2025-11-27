#pragma once

#include <string>
#include <string_view>

extern int hm2time(int hour, int minute);
extern std::string time2str(int time);
extern int timeAdd(int time, int minutes);
extern int dm2date(int day, int month);
extern int dm2day(int date);
extern int dm2month(int date);
extern int month2mday(int month);
extern int dm2yday(int date);

struct IOutput {
    virtual ~IOutput() = default;

    virtual void send(std::string_view topic, const std::string& message) = 0;
};

struct IMessageRouter {
    virtual ~IMessageRouter() = default;
    virtual void route(const std::string& topic, const std::string& payload) = 0;
};


#include "events.h"
#include "event_bus.hpp"
