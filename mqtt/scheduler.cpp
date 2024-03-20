#include <algorithm>
#include <functional>
#include "scheduler.hpp"


void TaskScheduler::schedule(Task task, std::chrono::system_clock::time_point time) {
    tasks.emplace_back(time, std::move(task));
}

void TaskScheduler::scheduleDaily(Task task, std::chrono::hours hour, std::chrono::minutes minute) {
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::hours>(now);
    auto next = today + hour + minute;

    if (next < now) {
        next += std::chrono::hours(24);
    }

    while (next < now + std::chrono::hours(24)) {
        schedule(task, next);
        next += std::chrono::hours(24);
    }
}

void TaskScheduler::run() {
    // std::sort(tasks.begin(), tasks.end());

    for (const auto& task : tasks) {
        // do not compile std::this_thread::sleep_until(task.first);
        task.second();
    }
}
