#pragma once
#include "../task.hpp"
#include "../event_data.hpp"

namespace task {

class TaskTemperature : public ITask {
public:
    explicit TaskTemperature(std::shared_ptr<IEventBus> eventBus)
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
        if (temperature.getFloat() != temp) {
            temperature.set(temp);
            evbus->publish(temperature);
        }
    }
    std::shared_ptr<IEventBus> evbus;
    EventData temperature {EventId::TEMPERATURE, 20.0f};
    int counter {0};
};

}  // namespace task
