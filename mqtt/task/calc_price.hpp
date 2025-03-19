#pragma once
#include "../task.hpp"
#include "../event_data.hpp"

namespace task {

class TaskCalcPrice : public Observable, public ITask {
public:
    TaskCalcPrice() = default;

    void execute() override {
        const int newPrice = 5;

        if (price.getInt() != newPrice) {
            price.set(newPrice);
            notify(price);
        }
    }

private:
    EventData price {EventId::ELECTRICITY_PRICE, 10};
};

}  // namespace task
