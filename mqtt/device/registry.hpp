#pragma once
#include <memory>
#include <string>
#include "device.hpp"
#include "../common.hpp"

namespace device {

class Registry {
public:
    explicit Registry(std::shared_ptr<IEventBus> evBus)
        : eventBus(evBus)
    {}
   
    void load(const std::string& filename);

    std::shared_ptr<Device> getDevice(const std::string& name) {
        if (auto it = devices_.find(name); it != devices_.end()) {
            return it->second;
        }
        return nullptr;
    }

    // bool subscribe(EventId eventId, IObserver& observer) override;

private:
    std::shared_ptr<IEventBus> eventBus;

    std::shared_ptr<Device> createDevice(
        const std::string& name,
        const std::string& type,
        const std::string& eventStr,
        std::shared_ptr<IEventBus> eventBus) const;
    std::unordered_map<std::string, std::shared_ptr<Device>> devices_;
};

}  // namespace device
