#include "lights.hpp"

namespace automation {

void Lights::setArg(const std::string& name, const std::string& value) {
    if (name == "on") {
        onTime = std::stoi(value);
    }
    else if (name == "off") {
        offTime = std::stoi(value);
    }
    else if (name == "brightness") {
        brightness = std::stoi(value);
    }
    else {
        throw std::runtime_error("Unknown argument: " + name);
    }
}

void Lights::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        int time = event.getInt();
        if (time >= onTime && time < offTime) {
            set(true);
        }
        else {
            set(false);
        }
    }
}

std::string Lights::toString() {
    std::string str_value = get() ? "ON" : "OFF";
    std::string result = "{\"state\": \"" + str_value + "\"";
    if (brightness != NAN_VALUE) {
        result += ", \"brightness\": " + std::to_string(brightness);
    }
    result += "}";
    return result;
}

}
