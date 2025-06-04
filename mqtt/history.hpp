#pragma once

#include "common.hpp"
#include "data/sourcesqlite.hpp"
#include "data/tables.hpp"

class History : public IObserver {
public:
    explicit History(data::SourceSqlite &history) :
        history(history)
    {}
    virtual ~History() = default;

    void onChange(const IEventData& value) override;
    void registerEvents(std::shared_ptr<IEventBus> evbus);

private:
    data::SourceSqlite &history;
    data::TableHistory table;
};
