#include "water_heater.hpp"

namespace controller {

void WaterHeater::onChange(const IValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        // implement the logic here
    }
}

}
