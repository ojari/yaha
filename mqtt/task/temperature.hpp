#pragma once
#include "../task.hpp"

namespace task {

class TaskTemperature : public ITask {
public:
    explicit TaskTemperature(EventBus& eventBus)
        : evbus {eventBus}
    {}

    TaskTemperature() = delete;

    void execute() override {
        if (counter < 30) {
            update(19.0f);
        } else if (counter < 60) {
            update(21.0f);
        } else {
            counter = 0;
        }
        counter++;
    }

private:
    void update(float temp) {
        if (temp != last_temp) {
            evbus.publish<TemperatureEvent>(TemperatureEvent("TaskTemp", temp));
            last_temp = temp;
        }
    }
    EventBus& evbus;
    float last_temp = -99.0;
    int counter {0};
};

}  // namespace task
