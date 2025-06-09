#pragma once
#include <memory>
#include <string>
#include "automation.hpp"

namespace automation {

struct LightValue {
    static const int NONE = -1;

    enum LightMode {
        FIXED,
        SUNRISE,
        SUNSET
    };

    LightValue(int _time) {
        set(_time);
    }

    void set(int value) {
        if (value == 9990) {
            mode = LightMode::SUNRISE;
        }
        else if (value == 9991) {
            mode = LightMode::SUNSET;
        }
        else {
            mode = LightMode::FIXED;
        }
        time = value;
    }

    friend bool operator>=(int lhs, const LightValue& rhs) {
        return lhs >= rhs.time;
    }

    friend bool operator<(int lhs, const LightValue& rhs) {
        return lhs < rhs.time;
    }

private:
    int time;
    LightMode mode = LightMode::FIXED;
};

struct Lights : public Automation {

    Lights(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name),
        onTime(LightValue::NONE),
        offTime(LightValue::NONE)
    {
        initial_value(false);
    }

    Lights(std::shared_ptr<IOutput> output, const std::string& name, int onTime, int offTime) :
        Automation(output, name),
        onTime(onTime),
        offTime(offTime)
    {
        initial_value(false);
    }

    void setArg(const std::string& name, const std::string& value) override;

    void registerEvents(std::shared_ptr<IEventBus> evbus) override {
        evbus->subscribe(EventId::TIME, this);
        evbus->subscribe(EventId::SUNRISE, this);
        evbus->subscribe(EventId::SUNSET, this);
    }

    std::string toString() override;

    void onChange(const IEventData& event) override;

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<Lights>(output, name);
    }

private:
    LightValue onTime;
    LightValue offTime;
    int brightness = NAN_VALUE;
};

}  // namespace automation
