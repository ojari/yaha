#include "task_manager.hpp"
#include "task/temperature.hpp"
#include "task/time.hpp"
#include "task/calc_price.hpp"

TaskManager::TaskManager() {
    tasks.emplace_back(std::make_unique<task::TaskTemperature>());
#ifdef DEBUG_TIME
    tasks.emplace_back(std::make_unique<task::TaskDebugTime>());
#else
    tasks.emplace_back(std::make_unique<task::TaskTime>());
#endif
    tasks.emplace_back(std::make_unique<task::TaskCalcPrice>());
}

void TaskManager::initialize() {
    for (const auto& task : tasks) {
        task->initialize();
    }
}

void TaskManager::execute() {
    for (const auto& task : tasks) {
        task->execute();
    }
}

bool TaskManager::subscribe(EventId eventId, IObserver& observer) {
    bool returned = false;
    // Check if the eventId is valid for any of the tasks
    for (const auto& task : tasks) {
        if (task->isValidEvent(eventId)) {

            if (auto observableTask = dynamic_cast<Observable*>(task.get())) {
                observableTask->subscribe(observer);
                returned = true;
            }
        }
    }
    return returned;
}
