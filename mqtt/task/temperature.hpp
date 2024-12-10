#pragma once
#include "../task.hpp"
#include "../event.hpp"

namespace task {

class TaskTemperature : public Observable, public ITask {
public:
    explicit TaskTemperature() = default;

    void execute() override {
        if (counter < 30) {
            update(19.0f);
        }
        else if (counter < 60) {
            update(21.0f);
        }
        else {
            counter = 0;
        }
        counter++;
    }
private:
    void update(float temp) {
        if (temperature.getFloat() != temp) {
            temperature.set(temp);
            notify(temperature);
        }
    }
    EventData temperature {EventId::TEMPERATURE, 20.0f};
    int counter {0};
};

}
