#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <nlohmann/json.hpp>
#include "data/data.hpp"

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

    void load(const std::string& filename);

    std::shared_ptr<Device> getDevice(const std::string& name) {
        auto it = devices_.find(name);
        if (it != devices_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
   /* void registerDevice(const std::string& name, std::shared_ptr<Device> device) {
        devices_[name] = device;
    }*/
    std::shared_ptr<Device> createDevice(const std::string& type) const;

    std::unordered_map<std::string, std::shared_ptr<Device>> devices_;
};
