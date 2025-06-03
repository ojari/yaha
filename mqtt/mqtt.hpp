#pragma once
#include <MQTTClient.h>
#include "common.hpp"
#include "task.hpp"
#include "device/registry.hpp"
#include "data/sourcesqlite.hpp"

class MessageRouter {
public:
    explicit MessageRouter(device::Registry *deviceReg)
        : deviceRegistry(deviceReg)
    {}

    void route(std::string& deviceName, std::string& payload);

    void bridge_msg(std::string& topic, std::string& payload);

private:
    device::Registry* deviceRegistry;
    bool verbose = false;
};


class Mqtt : public ITask, public IOutput {
public:
    explicit Mqtt(const std::string& filename, std::shared_ptr<IEventBus> evbus);

    void execute() override;
    void send(std::string_view topic, const std::string& message) override;

private:
    MQTTClient client;
    device::Registry deviceRegistry;
    MessageRouter messageRouter;
    int errorCounter = 0;
};
