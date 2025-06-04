#pragma once
#include "../task.hpp"
#include "../event_data.hpp"

namespace task {

class TaskCalcPrice : public ITask {
public:
    TaskCalcPrice(std::shared_ptr<IEventBus> evbus) :
        evbus(evbus)
    {
    }

    void execute() override {
        int randomChange = rand() % 3 - 1; // -1, 0, or 1
        int newPrice = price.getInt() + randomChange;

        if (price.set(newPrice)) {
            evbus->publish(price);
        }
    }

private:
    std::shared_ptr<IEventBus> evbus;
    EventData price {EventId::ELECTRICITY_PRICE, 10};
};

}  // namespace task
