#pragma once
#include <memory>
#include <string>
#include "automation.hpp"
#include "../device/registry.hpp"
#include "../common.hpp"
#include "../config.hpp"

namespace automation {

class MqttAutomationOutput : public IAutomationOutput {
public:
    explicit MqttAutomationOutput(
        std::shared_ptr<IOutput> output,
        const device::Registry* deviceRegistry = nullptr) :
        output(std::move(output)),
        deviceRegistry(deviceRegistry)
    {}

    void sendCommand(const std::string& deviceName, const CommandValues& values) override {
        if (deviceRegistry != nullptr) {
            if (const auto device = deviceRegistry->getByName(deviceName); device != nullptr) {
                const std::string topic = device->buildCommandTopic();
                const std::string devicePayload = device->buildCommandPayload(values.on, values.brightness);
                output->send(topic, devicePayload);
                return;
            }
        }

        const std::string topic = std::string(MQTT_TOPIC) + "/" + deviceName + "/set";
        const std::string state = values.on ? "ON" : "OFF";
        std::string payload = "{\"state\": \"" + state + "\"";
        if (values.brightness.has_value()) {
            payload.append(", \"brightness\": ");
            payload.append(std::to_string(*values.brightness));
        }
        payload.append("}");
        output->send(topic, payload);
    }

private:
    std::shared_ptr<IOutput> output;
    const device::Registry* deviceRegistry;
};

}  // namespace automation
