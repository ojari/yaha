#pragma once
#include <spdlog/spdlog.h>
#include "common.hpp"


struct DebugOutput : public IObserver {
    void onChange(const IEventData& value) override {
        if (value.id() == EventId::TIME) {
            time = value.getInt();
        } else if (value.id() == EventId::TEMPERATURE) {
            // do not show temperature changes
        } else {
            if (value.isInt()) {
                spdlog::info("{}: {} changed to {}", time2str(time), value.name(), value.getInt());
            } else {
                spdlog::info("{}: {} changed to {}", time2str(time), value.name(), value.getFloat());
            }
        }
    }

    void registerEvents(IEventManager& evManager) {
        const std::array<EventId, 12> events = {
            // EventId::TEMPERATURE,
            EventId::TEMPERATURE_ROOM,
            EventId::WEEKDAY,
            EventId::PROC_MEM,
            EventId::TIME,
            EventId::BUTTON_LIBRARY,
            EventId::BUTTON_LIVING_ROOM,
            EventId::SWITCH_AUTO_TALLI,
            EventId::SWITCH_AUTO_ULKO,
            EventId::SWITCH_VARASTO,
            EventId::SWITCH_PC_POWER,
            EventId::LAMP_LIVING_ROOM,
            EventId::LAMP_LIBRARY
        };

        for (const auto& event : events) {
            evManager.subscribe(event, *this);
        }
    }

private:
    int time {-1};
};
