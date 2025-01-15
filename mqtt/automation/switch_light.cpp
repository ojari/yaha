#include "switch_light.hpp"

namespace automation {

void SwitchLight::setArg(const std::string& name, const std::string& value) {
    if (name == "mode") {
        mode = std::stoi(value);
    }
    else if (name == "event") {
        buttonEvent = str2event(value);
    }
    else if (name == "eventLamp") {
        lampEvent = str2event(value);
    }
    else if (name == "brightness") {
        brightness = std::stoi(value);
    }
    else {
        throw std::runtime_error("Unknown argument: " + name);
    }
}

void SwitchLight::toggleLight() {
    if (getInt() == 0) {
        set(brightness);
    }
    else {
        set(0);
    }
}

void SwitchLight::onChange(const IEventData& event) {
    if (event.id() == buttonEvent) {
        switch (mode) {
            case 0: // off button toggles the light
                if (event.getInt() == 0) {
                    toggleLight();
                }
                break;
            case 1: // on button toggles the light
                if (event.getInt() == 1) {
                    toggleLight();
                }
                break;
            case 2: // both buttons works
                if (event.getInt()) {
                    set(brightness);
                }
                else {
                    set(0);
                }
                break;
        }
    }
    else if (event.id() == lampEvent) {
    }
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


}
