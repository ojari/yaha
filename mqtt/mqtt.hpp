#pragma once
#include "common.hpp"
#include "task.hpp"
#include "device/registry.hpp"
#include "data/sourcesqlite.hpp"

class MessageRouter {
public:
    MessageRouter(device::Registry *deviceReg) : 
        deviceRegistry(deviceReg)
    {}

    void route(std::string& deviceName, std::string& payload);

private:
    device::Registry* deviceRegistry;
    bool verbose = false;
};


class Mqtt : public ITask, public IOutput {
public:
    explicit Mqtt();
    void execute() override;
    void send(const std::string& topic, const std::string& message) override;

    IEventManager* getEventManager() {
        return &deviceRegistry;
    }
private:
    struct mosquitto *mosq;
    device::Registry deviceRegistry;
    MessageRouter messageRouter;
};