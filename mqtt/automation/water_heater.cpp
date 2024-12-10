#include "water_heater.hpp"

namespace automation {

void WaterHeater::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        // implement the logic here
    }
}

}
