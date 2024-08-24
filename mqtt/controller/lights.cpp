#include "lights.hpp"

namespace controller {

void Lights::onChange(const ValueItem& value) {
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
