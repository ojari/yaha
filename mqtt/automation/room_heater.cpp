#include "room_heater.hpp"

namespace automation {

void RoomHeater::onChange(const IValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        // implement the logic here
    }
}

}
