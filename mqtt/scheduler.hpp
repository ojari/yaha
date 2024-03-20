#pragma once

#include <chrono>
#include <vector>
//#include <thread>
#include <functional>

class TaskScheduler {
public:
    using Task = std::function<void()>;

    void schedule(Task task, std::chrono::system_clock::time_point time);
    void scheduleDaily(Task task, std::chrono::hours hour, std::chrono::minutes minute);
    void run();

private:
    std::vector<std::pair<std::chrono::system_clock::time_point, Task>> tasks;
};
