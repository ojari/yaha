#pragma once

#include <string>

extern int hm2time(int hour, int minute);
extern std::string time2str(int time);
extern int timeAdd(int time, int minutes);

struct IOutput {
    virtual ~IOutput() = default;

    virtual void send(std::string_view topic, const std::string& message) = 0;
};

enum class EventId {
  TEMPERATURE,
  TEMPERATURE_ROOM,
  ELECTRICITY_PRICE,
  TIME,
  WEEKDAY,
  SUNDOWN,

  BUTTON_LIVING_ROOM,
  BUTTON_LIBRARY,
  SWITCH_AUTO_TALLI,
  SWITCH_AUTO_ULKO,
  SWITCH_VARASTO,
  SWITCH_PC_POWER,

  LAMP_LIVING_ROOM,
  LAMP_LIBRARY,

  PROC_MEM,  // hardware_info
  USED_MEM,
  LOAD_AVG,
  CPU_TEMP,

  UNKNOWN
};

extern std::string event2str(EventId type);
extern EventId str2event(const std::string& str);

class IEventData {
public:
    virtual ~IEventData() = default;

    virtual  std::string name() const = 0;
    virtual EventId id() const = 0;
    virtual int getInt(int default_val = -1) const = 0;
    virtual float getFloat() const = 0;
    virtual bool isInt() const = 0;
    virtual void setv(IEventData &item) = 0;
    virtual void set(int aValue) = 0;
    virtual void set(float aValue) = 0;
};

struct IObserver {
    virtual ~IObserver() = default;

    virtual void onChange(const IEventData& value) = 0;
};

struct IEventManager {
    virtual ~IEventManager() = default;

    virtual bool subscribe(EventId eventId, IObserver& observer) = 0;
};


struct DualEventManager : IEventManager {
    DualEventManager(IEventManager* manager1, IEventManager* manager2) :
        manager1(manager1),
        manager2(manager2)
    {}
    virtual ~DualEventManager() = default;

    bool subscribe(EventId eventId, IObserver& observer) {
        if (eventId == EventId::UNKNOWN) {
            return true;
        }
        if (manager1->subscribe(eventId, observer) == false) {
            if (manager2->subscribe(eventId, observer) == false) {
                return false;
            }
            return true;
        }
        return true;
    }
private:
    IEventManager* manager1;
    IEventManager* manager2;
};
