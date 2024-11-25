#include "lights.hpp"

namespace automation {

void Lights::onChange(const IValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        int time = value.getInt();
        if (time >= onTime && time < offTime) {
            set(true);
        } else {
            set(false);
        }
    }
}

}
