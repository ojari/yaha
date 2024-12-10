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

}
