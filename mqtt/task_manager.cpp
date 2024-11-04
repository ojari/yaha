#include "task_manager.hpp"

void TaskManager::execute() {
    temperature.execute();
    time.execute();
    price.execute();
}

void TaskManager::subscribe(ETask taskId, IObserver& observer) {
    switch (taskId) {
        case ETask::TEMPERATURE:
            temperature.subscribe(observer);
            break;
        case ETask::TIME:
            time.subscribe(observer);
            break;
        case ETask::PRICE:
            price.subscribe(observer);
            break;
        default:
            break;
    }
}
