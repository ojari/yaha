#pragma once
#include "../task.hpp"
#include "../value.hpp"

namespace task {

class TaskCalcPrice : public Observable, public ITask {
public:
    explicit TaskCalcPrice() = default;

    void execute() override {
        price.set(5);
        notify(price);
    }
private:
    ValueItem price {ValueType::ELECTRICITY_PRICE, 10};

};

}