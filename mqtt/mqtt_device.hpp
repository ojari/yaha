#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

class Device {
public:
    virtual void on_message(std::string& deviceName, std::string& payload) = 0;
};


class LightDevice : public Device {
public:
    void on_message(std::string& deviceName, std::string& payload) override {
        std::cout << "LightDevice  " << deviceName << " :: " << payload << std::endl;
    }
};


class ThermostatDevice : public Device {
public:
    void on_message(std::string& deviceName, std::string& payload) override {
        std::cout << "Thermostat " << deviceName << " :: " << payload << std::endl;
    }
};


class DeviceRegistry {
public:
    void registerDevice(const std::string& name, Device* device) {
        devices_[name] = device;
    }

    Device* getDevice(const std::string& name) {
        auto it = devices_.find(name);
        if (it != devices_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, Device*> devices_;
};
