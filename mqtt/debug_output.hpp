#pragma once
#include <spdlog/spdlog.h>
#include "common.hpp"


struct DebugOutput {
    void registerEvents(EventBus& bus) {
        bus.subscribe<ButtonEvent>([&](const ButtonEvent& e) {
            spdlog::info("{}: Button {} = {}", time2str(time), e.location, e.pressed);
        });

        bus.subscribe<SwitchEvent>([&](const SwitchEvent& e) {
            spdlog::info("{}: Switch {} = {}", time2str(time), e.name, e.state);
        });

        bus.subscribe<LampEvent>([&](const LampEvent& e) {
            spdlog::info("{}: Lamp {} = {}", time2str(time), e.location, e.brightness);
        });

        bus.subscribe<TemperatureEvent>([&](const TemperatureEvent& e) {
            spdlog::info("{}: Temp {} = {}", time2str(time), e.room, e.value);
        });

        bus.subscribe<TimeEvent>([&](const TimeEvent& e) {
            time = e.GetTime();
        });


        /*const std::array<EventId, 18> events = {
            EventId::WEEKDAY,
            EventId::PROC_MEM,
            EventId::DARK,
            EventId::SUNRISE,
            EventId::SUNSET,
            EventId::YEAR,
	    EventId::DATE,
        };
        */
    }

private:
    int time {-1};
};
