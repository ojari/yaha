#pragma once

#include <vector>
#include "common.hpp"

struct Observable {
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
        // @TODO: use std::remove_if
        /* observers.erase(
            remove(observers.begin(), observers.end(), &observer),
            observers.end()
        );*/
    }

private:
    std::vector<IObserver*> observers;
};

