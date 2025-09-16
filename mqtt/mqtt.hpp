#pragma once
#include <MQTTClient.h>
#include "common.hpp"
#include "task.hpp"
#include "device/registry.hpp"
#include "data/sourcesqlite.hpp"

class Mqtt : public ITask, public IOutput {
public:
    explicit Mqtt(const std::string& filename, EventBus& evbus);

    void execute() override;
    void send(std::string_view topic, const std::string& message) override;

    /** Incoming MQTT message uses this method to route message to correct device
     */
    void route(std::string& deviceName, std::string& payload);

    /** Incoming bridge messages are handled here.
     */
    void bridge_msg(std::string& topic, std::string& payload);

private:
    MQTTClient client;
    device::Registry deviceRegistry;
    int errorCounter = 0;
};
