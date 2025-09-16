#pragma once
#include "../mqtt/common.hpp"
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
