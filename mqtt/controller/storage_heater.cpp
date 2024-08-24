#include "storage_heater.hpp"

namespace controller {

void StorageHeater::onChange(const ValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        // implement the logic here
    }
}

}
