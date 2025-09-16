#include "history.hpp"
#include <chrono>

void History::registerEvents(EventBus& bus) {
    bus.subscribe<TemperatureEvent>([&](const TemperatureEvent& e) {
        this->onEvent(e);
    });
    bus.subscribe<ElectricityPriceEvent>([&](const ElectricityPriceEvent& e) {
        this->onEvent(e);
    });
    bus.subscribe<ButtonEvent>([&](const ButtonEvent& e) {
        this->onEvent(e);
    });
    bus.subscribe<LampEvent>([&](const LampEvent& e) {
        this->onEvent(e);
    });
    bus.subscribe<SwitchEvent>([&](const SwitchEvent& e) {
        this->onEvent(e);
    });
}

void History::onEvent(const TemperatureEvent& e) {
/*    data::DataHistory data;

    data.epoch = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    data.event = value.id();
    data.val = value.getInt();

    dataToHeader(table, data);
    history.insert(table);
*/
}

void History::onEvent(const ElectricityPriceEvent& e) {
}

void History::onEvent(const ButtonEvent& e) {
}

void History::onEvent(const LampEvent& e) {
}

void History::onEvent(const SwitchEvent& e) {
}
