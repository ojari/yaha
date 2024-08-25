#pragma once
#include "common.hpp"
#include "device/registry.hpp"
#include "data/sourcesqlite.hpp"

class MessageRouter {
public:
    MessageRouter(device::Registry *deviceReg, DataInsertHistory &history) : 
        deviceRegistry(deviceReg),
        history(history)
    {}

    void route(std::string& deviceName, std::string& payload);

private:
    device::Registry* deviceRegistry;
    DataInsertHistory &history;
};


class Mqtt : public ITask, public IOutput {
public:
    explicit Mqtt(DataInsertHistory &history);
    void execute() override;
    void send(const std::string& topic, const std::string& message) override;
private:
    struct mosquitto *mosq;
    device::Registry deviceRegistry;
    MessageRouter messageRouter;
};