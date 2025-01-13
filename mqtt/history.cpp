#include "history.hpp"

void History::onChange(const IEventData& value) {
    DataHistory data;

    data.epoch = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    data.event = value.id();
    data.val = value.getInt();

    dataToHeader(table, data);
    history.insert(table);
}

void History::registerEvents(IEventManager &evman) {
    evman.subscribe(EventId::TEMPERATURE, *this);
    evman.subscribe(EventId::TEMPERATURE_ROOM, *this);
    evman.subscribe(EventId::ELECTRICITY_PRICE, *this);
    evman.subscribe(EventId::BUTTON_LIVING_ROOM, *this);
    evman.subscribe(EventId::BUTTON_LIBRARY, *this);
    evman.subscribe(EventId::LAMP_LIBRARY, *this);
    evman.subscribe(EventId::LAMP_LIVING_ROOM, *this);
    evman.subscribe(EventId::SWITCH_AUTO_TALLI, *this);
    evman.subscribe(EventId::SWITCH_AUTO_ULKO, *this);
}

