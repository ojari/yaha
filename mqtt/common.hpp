#pragma once

extern void showError(const char* message);
extern void showError(const char* message, const char* arg);
extern int hm2time(int hour, int minute);

/** Task is class that is called from main function constantly.
 */
struct Task {
    virtual ~Task() {}

    virtual void execute() = 0;
};