#pragma once
#include "device.hpp"

namespace device {

class SwitchDevice : public Device, public IDeviceBoolOut {
public:
    explicit SwitchDevice(const std::string& name, EventId eid) :
        Device(name, eid)
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
        // std::cout << "Switch " << deviceName << " :: " << state << std::endl;

        eventData.set((int)state);
        notify(eventData);
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
    bool state = false;
};

}