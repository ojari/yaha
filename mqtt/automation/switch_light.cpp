#include "switch_light.hpp"

namespace automation {

void SwitchLight::onChange(const IEventData& event) {
    if (event.id() == EventId::TIME) {
        // implement the logic here
    }
}

}
