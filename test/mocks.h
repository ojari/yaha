#pragma once
#include "../mqtt/common.hpp"
#include "../mqtt/event_data.hpp"
#include "../mqtt/event_bus.hpp"

// Mock output class for testing
struct MockOutput : public IOutput {
    std::string lastTopic;
    std::string lastPayload;

    void send(std::string_view topic, const std::string& payload) override {
        lastTopic = topic;
        lastPayload = payload;
    }
};

struct MockEventBus : public IEventBus {
    MockEventBus() = default;
    ~MockEventBus() = default;

    void subscribe(EventId eventId, IObserver* observer) override {
    }

    void unsubscribe(EventId eventId, IObserver* observer) override {
    }

    void publish(const IEventData& eventData)  {
        notified = true;
        lastValue = eventData.getInt();
    }

    bool notified = false;
    int lastValue = -1;
};
