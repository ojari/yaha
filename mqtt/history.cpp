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
    const std::array<EventId, 9> events = {
        EventId::TEMPERATURE,
        EventId::TEMPERATURE_ROOM,
        EventId::ELECTRICITY_PRICE,
        EventId::BUTTON_LIVING_ROOM,
        EventId::BUTTON_LIBRARY,
        EventId::LAMP_LIBRARY,
        EventId::LAMP_LIVING_ROOM,
        EventId::SWITCH_AUTO_TALLI,
        EventId::SWITCH_AUTO_ULKO
    };

    for (const auto& event : events) {
        evman.subscribe(event, *this);
    }
}

