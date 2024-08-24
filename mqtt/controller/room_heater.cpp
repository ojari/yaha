#include "room_heater.hpp"

namespace controller {

void RoomHeater::onChange(const ValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        // implement the logic here
    }
}

}
