#pragma once
#include <memory>
#include <string>
#include "device.hpp"

namespace device {

class Registry {
public:

    void load(const std::string& filename);

    std::shared_ptr<Device> getDevice(const std::string& name) {
        if (auto it = devices_.find(name); it != devices_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::shared_ptr<Device> createDevice(const std::string& name, const std::string& type) const;
    std::unordered_map<std::string, std::shared_ptr<Device>> devices_;
};

}