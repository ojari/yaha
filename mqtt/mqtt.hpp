#pragma once
#include <MQTTClient.h>
#include "common.hpp"
#include "task.hpp"
#include "device/registry.hpp"
#include "data/sourcesqlite.hpp"

class DeviceMessageRouter : public IMessageRouter {
public:
    explicit DeviceMessageRouter(const std::string& filename, EventBus& evbus);

    void route(const std::string& topic, const std::string& payload) override;

private:
    device::Registry deviceRegistry;
};


class BridgeMessageRouter : public IMessageRouter {
public:
	explicit BridgeMessageRouter() {}

    void route(const std::string& topic, const std::string& payload) override;

private:
};


class Mqtt : public ITask, public IOutput {
public:
    explicit Mqtt(IMessageRouter& devrouter, IMessageRouter& bridgerouter);

    void execute() override;
    void send(std::string_view topic, const std::string& message) override;

    /** Incoming MQTT message uses this method to route message to correct device
     */
    void route(std::string& deviceName, std::string& payload) {
		devRouter.route(deviceName, payload);
    }

    /** Incoming bridge messages are handled here.
     */
    void bridge_msg(std::string& topic, std::string& payload) {
		bridgeRouter.route(topic, payload);
    }

private:
    MQTTClient client;
    IMessageRouter& devRouter;
	IMessageRouter& bridgeRouter;
    int errorCounter = 0;
};
