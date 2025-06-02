#pragma once

#include "common.hpp"

/** Task is class that is called from main function constantly.
 */
struct ITask {
    virtual ~ITask() = default;

    virtual void initialize() {}

    virtual void execute() = 0;
};

struct IObservableTask : public ITask {
    virtual bool isValidEvent(EventId eventId) = 0;
};
