#include "mqtt.hpp"
#include "common.hpp"
#include "task_manager.hpp"
#include "actuator.hpp"
#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>

struct DebugOutput : public IObserver {
    void onChange(const ValueItem& value) override {
        if (value.getType() == ValueType::TIME) {
            time = value.getInt();
        }
        else if (value.getType() == ValueType::TEMPERATURE) {
            // do not show temperature changes
        }
        else {
            std::cout << time2str(time) << "  " << value.name() << " changed to ";
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
    Actuator actuator(dynamic_cast<IOutput*>(mqtt.get()));
    TaskManager taskManager;
    DebugOutput debugOutput;

    taskManager.subscribe(ETask::TEMPERATURE, debugOutput);
    taskManager.subscribe(ETask::TIME, debugOutput);

    while (true) {
        mqtt->execute();
        taskManager.execute();

        // std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0;
}
