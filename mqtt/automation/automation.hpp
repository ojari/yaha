#pragma once
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include "../common.hpp"
#include "facts.hpp"
#include "nlohmann/json.hpp"

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

struct IAutomationOutput {
    virtual ~IAutomationOutput() = default;
    struct CommandValues {
        bool on = false;
        std::optional<int> brightness = std::nullopt;
    };

    virtual void sendCommand(const std::string& deviceName, const CommandValues& values) = 0;
};

struct Automation {
    Automation(std::shared_ptr<IAutomationOutput> output, const std::string& name) :
        output(output),
        name(name)
    {}

    virtual void setArg(const std::string& name, const std::string& value) {
    }

    virtual void setCondition(const std::string& type, const nlohmann::json& config) {
    }

    virtual void registerEvents(EventBus& evbus) = 0;

    virtual IAutomationOutput::CommandValues commandValues() const {
        return { get(), std::nullopt };
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
    std::shared_ptr<IAutomationOutput> output;
    std::variant<int, bool> state;
    bool verbose = false;
    std::string name;
};

}  // namespace automation
