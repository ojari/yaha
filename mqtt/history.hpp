#pragma once

#include "common.hpp"
#include "data/sourcesqlite.hpp"
#include "data/tables.hpp"
#include <chrono>

class History : public IObserver {
public:
    History(SourceSqlite &history) :
        history(history)
    {}
    virtual ~History() = default;

    void onChange(const IEventData& value) override;
    void registerEvents(IEventManager &evman);

private:
    SourceSqlite &history;
    TableHistory table;
};
