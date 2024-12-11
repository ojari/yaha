#pragma once
#include "../task.hpp"
#include "../event_data.hpp"

namespace task {

class TaskCalcPrice : public Observable, public ITask {
public:
    explicit TaskCalcPrice() = default;

    void execute() override {
        price.set(5);
        notify(price);
    }
private:
    EventData price {EventId::ELECTRICITY_PRICE, 10};

};

}