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
                spdlog::info("{}: {} = {}", time2str(time), value.name(), value.getInt());
            } else {
                spdlog::info("{}: {} = {}", time2str(time), value.name(), value.getFloat());
            }
        }
    }

    void registerEvents(std::shared_ptr<IEventBus> bus) {
        const std::array<EventId, 18> events = {
            // EventId::TEMPERATURE,
            EventId::TEMPERATURE_ROOM,
            EventId::WEEKDAY,
            EventId::PROC_MEM,
            EventId::DARK,
            EventId::SUNRISE,
            EventId::SUNSET,
            EventId::TIME,
            EventId::YEAR,
			EventId::DATE,
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
            bus->subscribe(event, this);
        }
    }

private:
    int time {-1};
};
