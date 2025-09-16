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

std::string SwitchLight::toString() {
    std::string str_value = (getInt() != 0) ? "ON" : "OFF";
    std::string result = "{\"state\": \"" + str_value + "\"";
    if (get() && brightness != NAN_VALUE) {
        result += ", \"brightness\": " + std::to_string(brightness);
    }
    result += "}";
    return result;
}


}  // namespace automation
