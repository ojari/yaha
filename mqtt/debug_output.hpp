#pragma once
#include "common.hpp"
#include <spdlog/spdlog.h>


struct DebugOutput : public IObserver {
    void onChange(const IEventData& value) override {
        if (value.id() == EventId::TIME) {
            time = value.getInt();
        }
        else if (value.id() == EventId::TEMPERATURE) {
            // do not show temperature changes
        }
        else {
            if (value.isInt()) {
                spdlog::info("{}: {} changed to {}", time2str(time), value.name(), value.getInt());
            } else {
                spdlog::info("{}: {} changed to {}", time2str(time), value.name(), value.getFloat());
            }
        }
    }

    void registerEvents(IEventManager& evManager) {
        evManager.subscribe(EventId::TEMPERATURE, *this);
        evManager.subscribe(EventId::TEMPERATURE_ROOM, *this);
        evManager.subscribe(EventId::TIME, *this);
        evManager.subscribe(EventId::BUTTON_LIBRARY, *this);
        evManager.subscribe(EventId::BUTTON_LIVING_ROOM, *this);
       
    }

private:
    int time {-1};
};
