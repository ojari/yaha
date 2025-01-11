#include "lights.hpp"

namespace automation {

void Lights::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        int time = event.getInt();
        if (time >= onTime && time < offTime) {
            set(true);
        } else {
            set(false);
        }
    }
}

std::string Lights::toString() {
    std::string str_value = get() ? "ON" : "OFF";
    //std::string result = "{\"state\": \"" + str_value + "\", \"brightness\": 200}";
    std::string result = "{\"state\": \"" + str_value + "\"}";
    return result;
}

}
