#pragma once
#include "device.hpp"
#include "../event.hpp"

namespace device {

class SwitchDevice : public Device, public IDeviceBoolOut {
public:
    explicit SwitchDevice(const std::string& name, EventId eid) :
        deviceName(name),
        eventData(eid, 0)
    {}

    void on_message(std::string& _deviceName, nlohmann::json& payload) override {
        if (payload.contains("state")) {
            state = payload["state"].get<std::string>() == "ON";
        }
        else {
            const std::string& state_str = payload["action"].get<std::string>();
            if (state_str == "on") {
                state = true;
            } else if (state_str == "off") {
                state = false;
            }
            else {
                return;
            }
            state = payload["action"].get<std::string>() == "on";
        }
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
    EventData eventData;
    bool state = false;
};

}