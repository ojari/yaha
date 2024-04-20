#pragma once
#include "common.hpp"
#include "mqtt_device.hpp"
#include "database.hpp"

class MessageRouter {
public:
    MessageRouter(DeviceRegistry* deviceReg, DataInsertHistory &history) : 
        deviceRegistry(deviceReg),
        history(history)
    {}

    void route(std::string& deviceName, std::string& payload);

private:
    DeviceRegistry* deviceRegistry;
    DataInsertHistory &history;

    /*std::string extractDeviceName(const std::string& topic) {
        // Extract the device name from the topic
        std::size_t start = topic.find_first_of("/") + 1;
        std::size_t end = topic.find_first_of("/", start);
        return topic.substr(start, end - start);
    }*/
};


class Mqtt : public Task {
public:
    Mqtt(DataInsertHistory &history);
    void execute();

private:
    struct mosquitto *mosq;
    DeviceRegistry deviceRegistry;
    MessageRouter messageRouter;
};