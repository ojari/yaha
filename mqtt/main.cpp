#include "mqtt.hpp"
#include "common.hpp"
#include "collect.hpp"
#include "logic.hpp"
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


/**
 * @brief The main function of the program.
 */
int main() {
    std::unique_ptr<Database> db{createDatabase("data.db")};
    std::shared_ptr<Collect> collect{new Collect()};

    std::vector<Task*> tasks {
        new Mqtt(db->history),
        new SourceTime(collect),
        new SourceTemperature(collect)
    };

    DebugOutput debugOutput;
    collect->subscribe(debugOutput);


    while (true) {
        for (auto task : tasks)
            task->execute();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // cleanup
    //
    for (auto task : tasks) {
        delete task;
    }
    return 0;
}
