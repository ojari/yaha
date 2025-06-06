#pragma once
#include <memory>
#include <string>
#include <variant>
#include "../common.hpp"

namespace automation {

const int NAN_VALUE = -1;

enum AutomationType {
    LIGHTS,
    SWITCH,
    CAR_HEATER,
    STORAGE_HEATER,
    WATER_HEATER,
    ROOM_HEATER
};

struct Automation : public IObserver {
    Automation(std::shared_ptr<IOutput> output, const std::string& name) :
        output(output),
        name(name)
    {}

    virtual void setArg(const std::string& name, const std::string& value) {
    }

    virtual void registerEvents(std::shared_ptr<IEventBus> evbus) = 0;

    virtual std::string toString() {
        std::string str_value = get() ? "ON" : "OFF";
        std::string result = "{\"state\": \"" + str_value + "\"}";
        return result;
    }

    bool get() const {
        if (std::holds_alternative<int>(state)) {
            return std::get<int>(state) != 0;
        }

        return std::get<bool>(state);
    }

    int getInt() const {
        if (std::holds_alternative<bool>(state)) {
            return std::get<bool>(state);
        }
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

    void send(int value);
    void send(bool value);

private:
    std::shared_ptr<IOutput> output;
    std::variant<int, bool> state;
    bool verbose = false;
    std::string name;
};

}  // namespace automation
