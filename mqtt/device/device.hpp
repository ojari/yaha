#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "../common.hpp"
#include "../data/data.hpp"
#include "../event_data.hpp"
#include "../observable.hpp"

namespace device {

class Device : public Observable {
public:
    Device(const std::string& name, EventId eid) :
        deviceName(name),
        eventData(eid, 2)  // value that is not 0 or 1
    {}

    virtual ~Device() = default;

    virtual void on_message(std::string& deviceName, nlohmann::json& payload) = 0;
    virtual void getHistory(DataHistory &history) = 0;

    bool hasEvent(EventId eventId) const {
        return eventData.id() == eventId;
    }
protected:
    bool str2bool(const std::string& str) const {
        return str == "ON";
    }

    std::string deviceName;
    EventData eventData;
};

struct IDeviceBoolOut {
    virtual void send(IOutput& output, bool value) = 0;
};

struct IDeviceLightOut {
    virtual void send(IOutput& output, bool state, int brightness) = 0;
};

}