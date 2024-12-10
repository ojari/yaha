#pragma once

#include <string>

extern void showError(const char* message);
extern void showError(const char* message, const char* arg);
extern int hm2time(int hour, int minute);
extern std::string time2str(int time);
extern int timeAdd(int time, int minutes);

struct IOutput {
    virtual ~IOutput() = default;

    virtual void send(const std::string& topic, const std::string& message) = 0;
};

enum class EventId {
  TEMPERATURE,
  ELECTRICITY_PRICE,
  TIME,
  WEEKDAY,
  SUNRISE,
  SUNSET,
  BUTTON_LIVING_ROOM,
  BUTTON_LIBRARY,
  UNKNOWN
};

extern std::string event2str(EventId type);
extern EventId str2event(const std::string& str);

class IEventData {
public:
    virtual ~IEventData() = default;

    virtual  std::string name() const = 0;
    virtual EventId id() const = 0;
    virtual int getInt() const = 0;
    virtual float getFloat() const = 0;
    virtual bool isInt() const = 0;
    virtual void setv(IEventData &item) = 0;
    virtual void set(int aValue) = 0;
    virtual void set(float aValue) = 0;
};
