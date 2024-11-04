#pragma once
#include <memory>
#include <string>
#include "../actuator.hpp"
#include "../events.hpp"

namespace controller {

struct BooleanController : public IObserver {
    BooleanController(std::shared_ptr<IActuator> actuator, std::string_view name) :
        actuator(actuator),
        name(name)
    {}

    bool isOn() const {
        return state;
    }

    void setVerbose() {
        verbose = true;
    }

    std::string_view getName() const {
        return name;
    }

protected:
    void set(bool value);

private:
    std::shared_ptr<IActuator> actuator;
    bool state = false;
    bool verbose = false;
    std::string_view name;
};

}