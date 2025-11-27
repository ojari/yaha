#pragma once
#include "../task.hpp"

namespace task {

class TaskCalcPrice : public ITask {
public:
    TaskCalcPrice(EventBus& evbus) :
        evbus(evbus)
    {
    }

    void execute() override {
        int randomChange = rand() % 3 - 1; // -1, 0, or 1
        int newPrice = last_price + randomChange;

        if (newPrice != last_price) {
            evbus.publish(ElectricityPriceEvent(newPrice));

            last_price = newPrice;
        }
    }

private:
    EventBus& evbus;
    int last_price = -10;
};

}  // namespace task
