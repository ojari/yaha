#include "room_heater.hpp"

namespace automation {

void RoomHeater::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        // implement the logic here
    }
}

}
