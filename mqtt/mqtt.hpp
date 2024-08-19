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

    /*std::string extractDeviceName(const std::string& topic) {
        // Extract the device name from the topic
        std::size_t start = topic.find_first_of("/") + 1;
        std::size_t end = topic.find_first_of("/", start);
        return topic.substr(start, end - start);
    }*/
};


class Mqtt : public ITask, public IOutput {
public:
    Mqtt(DataInsertHistory &history);
    void execute() override;
    void send(const std::string& topic, const std::string& message) override;
private:
    struct mosquitto *mosq;
    device::Registry deviceRegistry;
    MessageRouter messageRouter;
};