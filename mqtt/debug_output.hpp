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
            //spdlog::info("{}: TempEvent {} = {}", time2str(time), e.room, e.value);
        });

        bus.subscribe<DateEvent>([&](const DateEvent& e) {
            spdlog::info("{}: DateEvent {}/{} {}  weekday = {}", time2str(time), e.day, e.month, e.year, e.weekday);
        });

        bus.subscribe<ElectricityPriceEvent>([&](const ElectricityPriceEvent& e) {
            // spdlog::info("{}: PriceEvent {}", time2str(time), e.price);
        });

        bus.subscribe<DarkEvent>([&](const DarkEvent& e) {
            spdlog::info("{}: DarkEvent {}", time2str(time), e.dark);
        });

        bus.subscribe<TimeEvent>([&](const TimeEvent& e) {
            time = e.GetTime();
        });
    }

private:
    int time {-1};
};
