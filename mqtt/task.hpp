#pragma once

#include "observer.hpp"

/** Task is class that is called from main function constantly.
 */
struct ITask {
    virtual ~ITask() = default;

    virtual void execute() = 0;
};

enum class ETask {
    TEMPERATURE,
    TIME,
    PRICE,
    MQTT
};

struct ITaskManager {
    virtual ~ITaskManager() = default;

    virtual void subscribe(ETask taskId, IObserver& observer) = 0;

    virtual void execute() = 0;
};
