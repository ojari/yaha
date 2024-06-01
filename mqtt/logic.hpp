#pragma once
#include "events.hpp"
#include "actuator.hpp"
#include <string>
#include <memory>
#include <functional>

struct DebugOutput : public Observer {
    void onChange(const IValues& state) override;
};

struct BooleanController : public Observer {
    BooleanController(std::shared_ptr<IActuator> actuator, const std::string& name) :
        actuator(actuator),
        state(false),
        name(name)
    {}

    virtual void onChange(const IValues& state) = 0;

    bool isOn() const {
        return state;
    }
protected:
    std::shared_ptr<IActuator> actuator;
    void set(bool value);
private:
    bool state;
    std::string name;
};

struct Lights : public BooleanController  {    
    Lights(std::shared_ptr<IActuator> actuator, int onTime, int offTime) :
        onTime(onTime),
        offTime(offTime),
        BooleanController(actuator, "Lights")
    {}
    void onChange(const IValues& vars) override;
private:
    int onTime;
    int offTime;
};

struct CarHeater : public BooleanController  {
    CarHeater(std::shared_ptr<IActuator> actuator, int leaveTime) :
        leaveTime(leaveTime),
        BooleanController(actuator, "CarHeater")
    {}
    void onChange(const IValues& state) override;
private:
    int calculateDuration(const IValues& state);
    int leaveTime;
    bool state;
};

class StorageHeater : public BooleanController  {
public:
    StorageHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "StorageHeater")
    {}
    void onChange(const IValues& state) override;
private:
    int calculateStartTime(const Values& state);
    int calculateEndTime(const Values& state);
};

class WaterHeater : public BooleanController  {
public:
    WaterHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "WaterHeater")
    {}
    void onChange(const IValues& state) override;
};

class RoomHeater : public BooleanController  {
public:
    RoomHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "RoomHeater")
    {}
    void onChange(const IValues& state) override;
};

class ControllerRegistry {
public:
    void registerCtrl(std::shared_ptr<BooleanController> output) {
        controllers.push_back(output);
    }

private:
    std::vector<std::shared_ptr<BooleanController>> controllers;

    std::shared_ptr<BooleanController> createController(const std::string& type, std::shared_ptr<IActuator> actuator, int onTime, int offTime, int leaveTime) {
        std::unordered_map<std::string, std::function<std::shared_ptr<BooleanController>()>> creators = {
            {"Lights", [&]() {
                return std::make_shared<Lights>(actuator, onTime, offTime);
            }},
            {"CarHeater", [&]() {
                return std::make_shared<CarHeater>(actuator, leaveTime);
            }},
            {"StorageHeater", [&]() {
                return std::make_shared<StorageHeater>(actuator);
            }},
            {"WaterHeater", [&]() {
                return std::make_shared<WaterHeater>(actuator);
            }},
            {"RoomHeater", [&]() {
                return std::make_shared<RoomHeater>(actuator);
            }}
        };

        auto it = creators.find(type);
        if (it != creators.end()) {
            return it->second();
        } else {
            // Handle unknown type
            return nullptr;
        }
    }
};