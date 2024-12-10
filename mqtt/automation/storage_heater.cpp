#include "storage_heater.hpp"

namespace automation {

void StorageHeater::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        // implement the logic here
    }
}

}
