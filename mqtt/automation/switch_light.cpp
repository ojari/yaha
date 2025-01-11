#include "switch_light.hpp"

namespace automation {

void SwitchLight::setArg(const std::string& name, const std::string& value) {
    if (name == "mode") {
        mode = std::stoi(value);
    }
    else if (name == "event") {
        buttonEvent = str2event(value);
    }
    else {
        throw std::runtime_error("Unknown argument: " + name);
    }
}
void SwitchLight::onChange(const IEventData& event) {
    if (event.id() == buttonEvent) {
        switch (mode) {
            case 0: // off button toggles the light
                if (event.getInt() == 0) {
                    set(!get());
                }
                break;
            case 1: // on button toggles the light
                if (event.getInt() == 1) {
                    set(!get());
                }
                break;
            case 2: // both buttons works
                set(event.getInt());
                break;
        }
        // implement the logic here
    }
}

}
