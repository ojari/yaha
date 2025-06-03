#pragma once

#include <vector>
#include "common.hpp"

/*struct Observable {
    void notify(const IEventData& value) const {
        for (auto observer : observers)
            observer->onChange(value);
    }

    void subscribe(IObserver& observer) {
        // Check if the observer is already subscribed
        for (auto obs : observers) {
            if (obs == &observer) {
                return; // Already subscribed
            }
        }

        observers.push_back(&observer);
    }

    void unsubscribe(IObserver& observer) {


private:
    std::vector<IObserver*> observers;
};
*/
