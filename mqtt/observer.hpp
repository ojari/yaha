#pragma once

#include <vector>
#include "common.hpp"

struct IObserver {
    virtual ~IObserver() = default;

    virtual void onChange(const IEventData& value) = 0;
};

struct Observable {
    void notify(const IEventData& value) const {
        for (auto observer : observers)
            observer->onChange(value);
    }

    void subscribe(IObserver& observer) {
        observers.push_back(&observer);
    }

    void unsubscribe(IObserver& observer) {
        //@TODO: use std::remove_if
        /*observers.erase(
            remove(observers.begin(), observers.end(), &observer),
            observers.end()
        );*/
    }

private:
    std::vector<IObserver*> observers;
};

