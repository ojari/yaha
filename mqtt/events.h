#pragma once
#include <string>

enum class EventId {
    Temperature = 0,
    Dark,
    Date,
    Time,
    Location,
    ElectricityPrice,
    Button,
    Switch,
    Lamp,
    Pc,
    UNKNOWN
};

#define DEFINE_EVENT(eventName) \
    static constexpr EventId ID = EventId::eventName; \
    EventId GetId() const override { return ID; } \
    const char* GetName() const override { return #eventName; } \
    eventName##Event() = delete;

struct EventBase {
    virtual ~EventBase() = default;
    virtual EventId GetId() const = 0;
    virtual const char* GetName() const = 0;
};

struct TemperatureEvent : public EventBase {
    DEFINE_EVENT(Temperature)
    TemperatureEvent(const std::string& room, float value) : room(room), value(value) {}
    std::string room;         // e.g. "LivingRoom"
    float value;              // °C
};

struct DarkEvent : public EventBase {
    DEFINE_EVENT(Dark)
    DarkEvent(bool isDark) : isDark(isDark) {}
    bool isDark;              // true = dark outside
};

struct DateEvent : public EventBase {
    DEFINE_EVENT(Date)
    DateEvent(int year, int month, int day, int weekday) : 
        year(year), 
        month(month), 
        day(day),
        weekday(weekday)
    {}
    int year, month, day;
    int weekday;
};

struct TimeEvent : public EventBase {
    DEFINE_EVENT(Time)
    TimeEvent(int ahour, int aminute) : hour(ahour), minute(aminute) {
        if (minute >= 60) {
            hour += minute / 60;
            minute %= 60;
        }
        if (minute < 0) {
            hour += minute / 60;
            minute = (minute % 60);
            if (minute != 0) {
                minute += 60;
                hour--;
            }
        }
        if (hour >= 24) {
            hour %= 24;
        }
        if (hour < 0) {
            hour += 24;
        }
    }
    int hour, minute;

    int GetTime() const {
        return hour * 100 + minute;
    }
};

struct LocationEvent : public EventBase {
    DEFINE_EVENT(Location)
    LocationEvent(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}
    double latitude;
    double longitude;
};

struct ElectricityPriceEvent : public EventBase {
    DEFINE_EVENT(ElectricityPrice)
    ElectricityPriceEvent(double price) : price(price) {}
    double price;             // €/kWh
};

struct ButtonEvent : public EventBase {
    DEFINE_EVENT(Button)
    ButtonEvent(const std::string& location, bool pressed) : location(location), pressed(pressed) {}
    std::string location;     // "LivingRoom" / "Library"
    bool pressed;
};

struct SwitchEvent : public EventBase {
    DEFINE_EVENT(Switch)
    SwitchEvent(const std::string& name, bool state) : name(name), state(state) {}
    std::string name;         // "AutoTalli", "AutoUlko", "Varasto", "PcPower"
    bool state;               // on/off
};

struct LampEvent : public EventBase {
    DEFINE_EVENT(Lamp)
    LampEvent(const std::string& location, int brightness) : location(location), brightness(brightness) {}
    std::string location;     // "LivingRoom" / "Library"
    int brightness;               // on/off
};

struct PcEvent : public EventBase {
    enum EventClass {
        process_memory,
        cpu_average
    };

    DEFINE_EVENT(Pc)
    PcEvent(EventClass eventType,
            unsigned long value)
        : eventType(eventType),
          value(value)
    {}
    EventClass eventType;
    unsigned long value;
};
