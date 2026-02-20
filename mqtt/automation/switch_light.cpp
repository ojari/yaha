#include "switch_light.hpp"
#include <stdexcept>

namespace automation {

void SwitchLight::setArg(const std::string& name, const std::string& value) {
    if (name == "mode") {
        mode = std::stoi(value);
    } else if (name == "event") {
        buttonLocation = value;
    } else if (name == "eventLamp") {
        lampLocation = value;
    } else if (name == "brightness") {
        brightness = std::stoi(value);
    } else {
        throw std::runtime_error("Unknown argument: " + name);
    }
}

void SwitchLight::toggleLight() {
    if (getInt() == 0) {
        send(brightness);
    } else {
        send(0);
    }
}

void SwitchLight::onButton(bool pressed)
{
    switch (mode) {
    case 0:  // off button toggles the light
        if (!pressed) {
            toggleLight();
        }
        break;
    case 1:  // on button toggles the light
        if (pressed) {
            toggleLight();
        }
        break;
    case 2:  // both buttons works
        if (pressed) {
            send(brightness);
        }
        else {
            send(0);
        }
        break;
    }
}

void SwitchLight::onLamp(int brightness)
{
    initial_value(brightness);
}

IAutomationOutput::CommandValues SwitchLight::commandValues() const {
    IAutomationOutput::CommandValues values;
    const int current = getInt();
    values.on = current != 0;
    if (values.on) {
        values.brightness = current;
    }
    return values;
}


}  // namespace automation
