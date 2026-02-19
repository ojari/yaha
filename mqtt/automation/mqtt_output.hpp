#pragma once
#include <memory>
#include <string>
#include "automation.hpp"
#include "../common.hpp"
#include "../config.hpp"

namespace automation {

class MqttAutomationOutput : public IAutomationOutput {
public:
    explicit MqttAutomationOutput(std::shared_ptr<IOutput> output) :
        output(std::move(output))
    {}

    void sendCommand(const std::string& deviceName, const std::string& payload) override {
        const std::string topic = std::string(MQTT_TOPIC) + "/" + deviceName + "/set";
        output->send(topic, payload);
    }

private:
    std::shared_ptr<IOutput> output;
};

}  // namespace automation
