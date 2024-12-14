#pragma once

#include "common.hpp"
#include "data/data.hpp"

class History : public IObserver {
public:
    History(DataInsertHistory &history) :
        history(history)
    {}
    virtual ~History() = default;

    void onChange(const IEventData& value) override {
        DataHistory entry;
        if (value.id() == EventId::TEMPERATURE) {
            temperature = value.getFloat();
        }
        else if (value.id() == EventId::TEMPERATURE_ROOM) {
            temperatureRoom = value.getFloat();
        }
        else if (value.id() == EventId::ELECTRICITY_PRICE) {
            electricityPrice = value.getFloat();
        }
        else if (value.id() == EventId::BUTTON_LIVING_ROOM) {
            buttonLivingRoom = value.getInt();
        }
        else if (value.id() == EventId::BUTTON_LIBRARY) {
            buttonLibrary = value.getInt();
        }
        else if (value.id() == EventId::SWITCH_AUTO_TALLI) {
            switchAutoTalli = value.getInt();
        }
        else if (value.id() == EventId::SWITCH_AUTO_ULKO) {
            switchAutoUlko = value.getInt();
        }
        history.insert(entry);
    }

    void subscribeEvents(IEventManager &evman) {
        evman.subscribe(EventId::TEMPERATURE, *this);
        evman.subscribe(EventId::TEMPERATURE_ROOM, *this);
        evman.subscribe(EventId::ELECTRICITY_PRICE, *this);
        evman.subscribe(EventId::BUTTON_LIVING_ROOM, *this);
        evman.subscribe(EventId::BUTTON_LIBRARY, *this);
        evman.subscribe(EventId::SWITCH_AUTO_TALLI, *this);
        evman.subscribe(EventId::SWITCH_AUTO_ULKO, *this);
    }

private:
    DataInsertHistory &history;

    float temperature;
    float temperatureRoom;
    float electricityPrice;
    int buttonLivingRoom;
    int buttonLibrary;
    int switchAutoTalli;
    int switchAutoUlko;
};
