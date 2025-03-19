#pragma once
#include "common.hpp"
#include "task/temperature.hpp"
#include "task/time.hpp"
#include "task/calc_price.hpp"

struct TaskManager : public IEventManager {
    TaskManager() = default;

    void initialize() {
        time.initialize();
    }

    void execute();

    bool subscribe(EventId eventId, IObserver& observer) override;

private:
    task::TaskTemperature temperature;
#ifdef DEBUG_TIME
    task::TaskDebugTime time;
#else
    task::TaskTime time;
#endif
    task::TaskCalcPrice price;
};
