#include "storage_heater.hpp"

namespace automation {

void StorageHeater::onChange(const IValueItem& value) {
    if (value.getType() == ValueType::TIME) {
        // implement the logic here
    }
}

}
