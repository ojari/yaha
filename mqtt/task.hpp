#pragma once

/** Task is class that is called from main function constantly.
 */
struct ITask {
    virtual ~ITask() = default;

    virtual void initialize() {}

    virtual void execute() = 0;
};
