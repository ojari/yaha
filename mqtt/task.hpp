#pragma once

/** Task is class that is called from main function constantly.
 */
struct ITask {
    virtual ~ITask() = default;

    virtual void execute() = 0;
};
