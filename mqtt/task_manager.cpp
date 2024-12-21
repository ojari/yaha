#include "task_manager.hpp"

void TaskManager::execute() {
    temperature.execute();
    time.execute();
    price.execute();
}

bool TaskManager::subscribe(EventId eventId, IObserver& observer) {
    switch (eventId) {
        case EventId::TEMPERATURE:
            temperature.subscribe(observer);
            break;
        case EventId::SUNRISE:
        case EventId::SUNSET:
        case EventId::TIME:
            time.subscribe(observer);
            break;
        case EventId::ELECTRICITY_PRICE:
            price.subscribe(observer);
            break;
        default:
            return false;
            break;
    }
    return true;
}
