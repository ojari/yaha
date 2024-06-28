#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <nlohmann/json.hpp>
#include "data/data.hpp"
#include "common.hpp"

class Device {
public:
    virtual ~Device() = default;

    virtual void on_message(std::string& deviceName, nlohmann::json& payload) = 0;
    virtual void getHistory(DataHistory &history) = 0;
protected:
    bool str2bool(const std::string& str) const {
        return str == "ON";
    }
};

struct IDeviceBoolOut {
    virtual void send(IOutput& output, bool value) = 0;
};

struct IDeviceLightOut {
    virtual void send(IOutput& output, bool state, int brightness) = 0;
};

class LightDevice : public Device, public IDeviceLightOut {
public:
    explicit LightDevice(const std::string& name) :
        deviceName(name)
    {
    }
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        brightness = payload["brightness"];
        state = str2bool(payload["state"]);
        std::cout << "Light  " << deviceName << " :: " << state << " :: " << brightness << std::endl;
    }
    void getHistory(DataHistory &history) override {
        history.type = DataType::LIGHT;
        history.val1 = static_cast<int>(state);
    }
    void send(IOutput& output, bool value, int newBrightness) override {
        std::string topic = "zigbee2mqtt/" + deviceName + "/set";
        std::string payload;
        payload.append(R"({"state": ")");
        payload.append(value ? "ON" : "OFF");
        payload.append(R"(", "brightness": )");
        payload.append(std::to_string(newBrightness));
        payload.append("}");
        output.send(topic, payload);
    }

private:
    int brightness = 0;
    bool state = false;
    std::string deviceName;
};


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


class TempSensorDevice : public Device {
public:
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        temperature = payload["temperature"];
        humidity = payload["humidity"];
        std::cout << "Temperature " << deviceName << " :: " << temperature << "C :: " << humidity << "%" << std::endl;
    }
    void getHistory(DataHistory &history) override {
        history.type = DataType::TEMPERATURE;
        history.val1 = static_cast<int>(temperature);
        history.val2 = static_cast<int>(humidity);
    }
private:
    float temperature;
    float humidity;
};


class DeviceRegistry {
public:

    void load(const std::string& filename);

    std::shared_ptr<Device> getDevice(const std::string& name) {
        if (auto it = devices_.find(name); it != devices_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::shared_ptr<Device> createDevice(const std::string& name, const std::string& type) const;
    std::unordered_map<std::string, std::shared_ptr<Device>> devices_;
};
