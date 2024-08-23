#include "mqtt.hpp"
#include "common.hpp"
#include "task/time.hpp"
#include "task/temperature.hpp"
#include "logic.hpp"
#include "actuator.hpp"
#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>

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
    Actuator actuator(dynamic_cast<IOutput*>(mqtt.get()));

    auto task_time = std::make_unique<task::TaskTime>();
    auto task_temp = std::make_unique<task::TaskTemperature>();

    std::vector<ITask*> tasks {
        dynamic_cast<ITask*>(mqtt.get()),
        dynamic_cast<ITask*>(task_time.get()),
        dynamic_cast<ITask*>(task_temp.get())
    };
    
    DebugOutput debugOutput;
    task_temp->subscribe(debugOutput);
    task_time->subscribe(debugOutput);

    while (true) {
        for (const auto& task : tasks)
            task->execute();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    return 0;
}
