#pragma once
#include "task.hpp"
#include <vector>
#include "task/temperature.hpp"
#include "task/time.hpp"
#include "task/calc_price.hpp"

struct TaskManager : public ITaskManager {
    TaskManager() = default;

    void execute() override;

    void subscribe(ETask taskId, IObserver& observer) override;

private:
    task::TaskTemperature temperature;
    task::TaskTime time;
    task::TaskCalcPrice price;
};