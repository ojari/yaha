#pragma once
#include <spdlog/spdlog.h>
#include "common.hpp"


struct DebugOutput {
    void registerEvents(EventBus& bus) {
        bus.subscribe<SwitchEvent>([&](const SwitchEvent& e) {
            spdlog::info("{}: SwitchEvent {} = {}", time2str(time), e.name, e.state);
        });

        bus.subscribe<LampEvent>([&](const LampEvent& e) {
            spdlog::info("{}: LampEvent {} = {}", time2str(time), e.location, e.brightness);
        });

        bus.subscribe<TemperatureEvent>([&](const TemperatureEvent& e) {
            spdlog::info("{}: TempEvent {} = {}", time2str(time), e.room, e.value);
        });

        bus.subscribe<TimeEvent>([&](const TimeEvent& e) {
            time = e.GetTime();
        });
    }

private:
    int time {-1};
};
