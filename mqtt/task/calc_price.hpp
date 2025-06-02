#pragma once
#include "../task.hpp"
#include "../event_data.hpp"

namespace task {

class TaskCalcPrice : public Observable, public IObservableTask {
public:
    TaskCalcPrice() = default;

    void execute() override {
        // random number between -1 and 1
        int randomChange = rand() % 3 - 1; // -1, 0, or 1
        int newPrice = price.getInt() + randomChange;

        if (price.set(newPrice)) {
            notify(price);
        }
    }

    bool isValidEvent(EventId eventId) override {
        return (eventId == price.id());
    }

private:
    EventData price {EventId::ELECTRICITY_PRICE, 10};
};

}  // namespace task
