#pragma once
#include "events.hpp"
#include <vector>
#include "task/temperature.hpp"
#include "task/time.hpp"

struct TaskManager : public ITaskManager {
    TaskManager() = default;

    void execute() override;

    void subscribe(ETask taskId, IObserver& observer) override;

private:
    task::TaskTemperature temperature;
    task::TaskTime time;
};