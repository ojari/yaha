#pragma once

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
    void registerEvents(std::shared_ptr<IEventBus> evbus);

private:
    SourceSqlite &history;
    TableHistory table;
};
