#include "water_heater.hpp"

namespace automation {

void WaterHeater::onChange(const IValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        // implement the logic here
    }
}

}
