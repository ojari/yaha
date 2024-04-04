#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <nlohmann/json.hpp>

class Device {
public:
    virtual void on_message(std::string& deviceName, nlohmann::json& payload) = 0;
};


class LightDevice : public Device {
public:
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        int brightness = payload["brightness"];
        std::string state = payload["state"];
        std::cout << "Light  " << deviceName << " :: " << state << " :: " << brightness << std::endl;
    }
};


class SwitchDevice : public Device {
public:
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        std::string state = payload["state"];
        std::cout << "Switch " << deviceName << " :: " << state << std::endl;
    }
};


class TempSensorDevice : public Device {
public:
    void on_message(std::string& deviceName, nlohmann::json& payload) override {
        std::cout << "Temperature " << deviceName << " :: " << payload << std::endl;
    }
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
