#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <nlohmann/json.hpp>
#include "data.hpp"

class Device {
public:
    virtual void on_message(std::string& deviceName, nlohmann::json& payload) = 0;
    virtual void getHistory(DataHistory &history) = 0;
protected:
    bool str2bool(const std::string& str) {
        return str == "ON";
    }
};


class LightDevice : public Device {
public:
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        brightness = payload["brightness"];
        state = str2bool(payload["state"]);
        std::cout << "Light  " << deviceName << " :: " << state << " :: " << brightness << std::endl;
    }
    void getHistory(DataHistory &history) override {
        history.type = DataType::LIGHT;
        history.val1 = static_cast<int>(state);
    }

private:
    int brightness;
    bool state;
};


class SwitchDevice : public Device {
public:
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        state = str2bool(payload["state"]);
        std::cout << "Switch " << deviceName << " :: " << state << std::endl;
    }
    void getHistory(DataHistory &history) override {
        history.type = DataType::SWITCH;
        history.val1 = static_cast<int>(state);
    }
private:
    bool state;
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
    void registerDevice(const std::string& name, Device* device) {
        devices_[name] = device;
    }

    void load(const std::string& filename);

    Device* getDevice(const std::string& name) {
        auto it = devices_.find(name);
        if (it != devices_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, Device*> devices_;
    LightDevice lightDevice;
    SwitchDevice switchDevice;
    TempSensorDevice tempSensorDevice;
};
