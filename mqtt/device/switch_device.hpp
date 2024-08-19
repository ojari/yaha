#pragma once
#include "device.hpp"

namespace device {

class SwitchDevice : public Device, public IDeviceBoolOut {
public:
    explicit SwitchDevice(const std::string& name) :
        deviceName(name)
    {}
    void on_message(std::string& _deviceName, nlohmann::json& payload) override {
        state = str2bool(payload["state"]);
        std::cout << "Switch " << deviceName << " :: " << state << std::endl;
    }
    void getHistory(DataHistory &history) override {
        history.type = DataType::SWITCH;
        history.val1 = static_cast<int>(state);
    }
    void send(IOutput& output, bool value) override {
        std::string topic = "zigbee2mqtt/" + deviceName + "/set";
        std::string payload;
        payload.append(R"({"state": ")");
        payload.append(value ? "ON" : "OFF");
        payload.append("\"}");
        output.send(topic, payload);
    }
private:
    std::string deviceName;
    bool state = false;
};

}