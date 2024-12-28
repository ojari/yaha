#pragma once
#include <memory>
#include <string>
#include <variant>
#include "../actuator.hpp"
#include "../common.hpp"

namespace automation {

enum AutomationType {
    LIGHTS,
    SWITCH,
    CAR_HEATER,
    STORAGE_HEATER,
    WATER_HEATER,
    ROOM_HEATER
};

struct Automation : public IObserver {
    Automation(std::shared_ptr<IActuator> actuator, const std::string& name) :
        actuator(actuator),
        name(name)
    {}

    virtual void registerEvents(IEventManager& evman) = 0;

    virtual std::string toString() {
        std::string str_value = get() ? "ON" : "OFF";
        std::string result = "{\"state\": \"" + str_value + "\"}";
        return result;
    }

    bool get() const {
        return std::get<bool>(state);
    }

    int getInt() const {
        return std::get<int>(state);
    }

    void setVerbose() {
        verbose = true;
    }

    std::string_view getName() const {
        return name;
    }

protected:
    void initial_value(int value);
    void initial_value(bool value);

    void set(int value);
    void set(bool value);

private:
    std::shared_ptr<IActuator> actuator;
    std::variant<int, bool> state;
    bool verbose = false;
    std::string name;
};

};