#pragma once

#include <string>

extern void showError(const char* message);
extern void showError(const char* message, const char* arg);
extern int hm2time(int hour, int minute);
extern int timeAdd(int time, int minutes);

/** Task is class that is called from main function constantly.
 */
struct ITask {
    virtual ~ITask() = default;

    virtual void execute() = 0;
};

struct IOutput {
    virtual ~IOutput() = default;

    virtual void send(const std::string& topic, const std::string& message) = 0;
};