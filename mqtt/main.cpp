#include "mqtt.hpp"
#include "common.hpp"
#include "task_manager.hpp"
#include "actuator.hpp"
#include "automation/registry.hpp"
#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>

struct DebugOutput : public IObserver {
    void onChange(const IEventData& value) override {
        if (value.id() == EventId::TIME) {
            time = value.getInt();
        }
        else if (value.id() == EventId::TEMPERATURE) {
            // do not show temperature changes
        }
        else {
            std::cout << time2str(time) << " " << value.name() << " changed to ";
            if (value.isInt()) {
                std::cout << value.getInt();
            } else {
                std::cout << value.getFloat();
            }
            std::cout << std::endl;
        }
    }

private:
    int time {-1};
};


class IntTime {
public:
    IntTime(int hour_, int minute_) :
        hour(hour_),
        minute(minute_)
    {}
    int getHour() const {
        return hour;
    }
    int getMinute() const {
        return minute;
    }
    int getTime() const {
        return hm2time(hour, minute);
    }

    void operator+(int minutes) {
        minute += minutes;
        if (minute >= 60) {
            hour += minute / 60;
            minute %= 60;
            if (hour >= 24) {
                hour %= 24;
            }
        }
    }
private:
    int hour;
    int minute;
};

struct DummyHistory : public DataInsertHistory {
    void insert(const DataHistory& data) override {
        // dummy implementation: do nothing
    }
};


/**
 * @brief The main function of the program.
 */
int main() {
    DummyHistory history;
    auto mqtt = std::make_unique<Mqtt>(history);
    auto actuator = std::make_shared<Actuator>(dynamic_cast<IOutput*>(mqtt.get()));
    TaskManager taskManager;
    automation::Registry automations;
    DebugOutput debugOutput;
    DualEventManager evManager(&taskManager, mqtt->getEventManager());

    evManager.subscribe(EventId::TEMPERATURE, debugOutput);
    evManager.subscribe(EventId::TIME, debugOutput);
    evManager.subscribe(EventId::BUTTON_LIBRARY, debugOutput);
    evManager.subscribe(EventId::BUTTON_LIVING_ROOM, debugOutput);
    evManager.subscribe(EventId::TIME, *actuator);

    std::vector<int> timeRange1 = {1000, 1300};
    std::vector<int> timeRange2 = {1200, 1500};
    automations.add(evManager, "Demo1", automation::LIGHTS, actuator, timeRange1);
    automations.add(evManager, "Demo2", automation::LIGHTS, actuator, timeRange2);

    automations.add(evManager, "ButtonKirjasto2", automation::SWITCH, actuator, timeRange1);

    //automations.add(evManager, actuator,  ConfigController { "Demo1", "Lights", "", 1000, 1300 });
    //automations.add(evManager, actuator,  ConfigController { "Demo2", "Lights", "", 1200, 1500 });
    while (true) {
        mqtt->execute();
        //taskManager.execute();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0;
}
