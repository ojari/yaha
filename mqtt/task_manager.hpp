#pragma once
#include "common.hpp"
#include "task/temperature.hpp"
#include "task/time.hpp"
#include "task/calc_price.hpp"

struct TaskManager : public IEventManager {
    TaskManager() = default;

    void execute();

    bool subscribe(EventId eventId, IObserver& observer) override;

private:
    task::TaskTemperature temperature;
    task::TaskTime time;
    task::TaskCalcPrice price;
};