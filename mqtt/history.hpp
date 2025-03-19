#pragma once

#include <chrono>
#include "common.hpp"
#include "data/sourcesqlite.hpp"
#include "data/tables.hpp"

class History : public IObserver {
public:
    explicit History(SourceSqlite &history) :
        history(history)
    {}
    virtual ~History() = default;

    void onChange(const IEventData& value) override;
    void registerEvents(IEventManager &evman);

private:
    SourceSqlite &history;
    TableHistory table;
};
