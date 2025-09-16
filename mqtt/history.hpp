#pragma once

#include "common.hpp"
#include "data/sourcesqlite.hpp"
#include "data/tables.hpp"

class History {
public:
    explicit History(data::SourceSqlite &history) :
        history(history)
    {}
    virtual ~History() = default;

    void registerEvents(EventBus& evbus);

private:
    void onEvent(const TemperatureEvent& e);
    void onEvent(const ElectricityPriceEvent& e);
    void onEvent(const ButtonEvent& e);
    void onEvent(const LampEvent& e);
    void onEvent(const SwitchEvent& e);

    data::SourceSqlite &history;
    data::TableHistory table;
};
