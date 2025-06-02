#pragma once
#include "common.hpp"
#include "task.hpp"
#include <vector>
#include <memory>

struct TaskManager : public IEventManager {
    TaskManager();

    void initialize();
    void execute();
    bool subscribe(EventId eventId, IObserver& observer) override;

private:
    std::vector<std::unique_ptr<IObservableTask>> tasks;

};
