#pragma once
#include "mqtt_device.hpp"

class MessageRouter {
public:
    MessageRouter(DeviceRegistry* deviceReg) : deviceRegistry(deviceReg) {}

    void route(std::string& deviceName, std::string& payload);

private:
    DeviceRegistry* deviceRegistry;

    /*std::string extractDeviceName(const std::string& topic) {
        // Extract the device name from the topic
        std::size_t start = topic.find_first_of("/") + 1;
        std::size_t end = topic.find_first_of("/", start);
        return topic.substr(start, end - start);
    }*/
};


class Mqtt {
public:
    Mqtt();
    void loop();

private:
    void readDevices();

    struct mosquitto *mosq;
    LightDevice lightDevice;
    ThermostatDevice thermostatDevice;
    DeviceRegistry deviceRegistry;
    MessageRouter messageRouter;

};