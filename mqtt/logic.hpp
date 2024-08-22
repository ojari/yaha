#pragma once
#include "events.hpp"
#include "actuator.hpp"
#include <string>
#include <memory>
#include <functional>

struct DebugOutput : public Observer {
    void onChange(const ValueItem& value) override;
};

struct BooleanController : public Observer {
    BooleanController(std::shared_ptr<IActuator> actuator, const std::string& name) :
        actuator(actuator),
        name(name)
    {}

    bool isOn() const {
        return state;
    }

    void setVerbose() {
        verbose = true;
    }

protected:
    void set(bool value);

private:
    std::shared_ptr<IActuator> actuator;
    bool state = false;
    bool verbose = false;
    std::string name;
};

struct Lights : public BooleanController  {    
    Lights(std::shared_ptr<IActuator> actuator, int onTime, int offTime) :
        BooleanController(actuator, "Lights"),
        onTime(onTime),
        offTime(offTime)
    {}
    void onChange(const ValueItem& value) override;
private:
    int onTime;
    int offTime;
};

struct CarHeater : public BooleanController  {
    CarHeater(std::shared_ptr<IActuator> actuator, int leaveTime) :
        BooleanController(actuator, "CarHeater"),
        leaveTime(leaveTime)
    {}
    void onChange(const ValueItem& value) override;
private:
    int calculateDuration(const ValueItem& value) const;
    int leaveTime;
    int offset = 60;
    bool state;
};

class StorageHeater : public BooleanController  {
public:
    explicit StorageHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "StorageHeater")
    {}
    void onChange(const ValueItem& value) override;
private:
    int calculateStartTime(const ValueItem& value);
    int calculateEndTime(const ValueItem& value);
};

class WaterHeater : public BooleanController  {
public:
    explicit WaterHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "WaterHeater")
    {}
    void onChange(const ValueItem& value) override;
};

class RoomHeater : public BooleanController  {
public:
    explicit RoomHeater(std::shared_ptr<IActuator> actuator) :
        BooleanController(actuator, "RoomHeater")
    {}
    void onChange(const ValueItem& value) override;
};

class ControllerRegistry {
public:
    void registerCtrl(std::shared_ptr<BooleanController> output) {
        controllers.push_back(output);
    }

private:
    std::vector<std::shared_ptr<BooleanController>> controllers;

    std::shared_ptr<BooleanController> createController(
        const std::string& type,
        std::shared_ptr<IActuator> actuator,
        int onTime,
        int offTime,
        int leaveTime) const
    {
        std::unordered_map<std::string, std::function<std::shared_ptr<BooleanController>()>> creators = {
            {"Lights", [actuator, onTime, offTime]() {
                return std::make_shared<Lights>(actuator, onTime, offTime);
            }},
            {"CarHeater", [actuator, leaveTime]() {
                return std::make_shared<CarHeater>(actuator, leaveTime);
            }},
            {"StorageHeater", [actuator]() {
                return std::make_shared<StorageHeater>(actuator);
            }},
            {"WaterHeater", [actuator]() {
                return std::make_shared<WaterHeater>(actuator);
            }},
            {"RoomHeater", [actuator]() {
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