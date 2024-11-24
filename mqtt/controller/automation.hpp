#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <variant>
#include "../actuator.hpp"
#include "../observer.hpp"

namespace controller {

struct Automation : public IObserver {
    Automation(std::shared_ptr<IActuator> actuator, std::string_view name) :
        actuator(actuator),
        name(name)
    {}

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
    void set(int value);
    void set(bool value);

private:
    std::shared_ptr<IActuator> actuator;
    std::variant<int, bool> state;
    bool verbose = false;
    std::string_view name;
};

};