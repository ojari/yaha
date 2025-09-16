#pragma once
#include <memory>
#include <string>
#include "automation.hpp"

namespace automation {

struct SwitchLight : public Automation {
    SwitchLight(std::shared_ptr<IOutput> output, const std::string& name) :
        Automation(output, name)
    {
        initial_value(0);
    }

    void setArg(const std::string& name, const std::string& value) override;

    std::string toString() override;

    void registerEvents(EventBus& evbus) override {
        evbus.subscribe<ButtonEvent>([&](const ButtonEvent& e) {
            if (e.location == buttonLocation) {
                onButton(e.pressed);
            }
        });

        evbus.subscribe<LampEvent>([&](const LampEvent& e) {
            if (e.location == lampLocation) {
                onLamp(e.brightness);
            }
        });
    }

    static std::shared_ptr<Automation> create(
        const std::string& name,
        std::shared_ptr<IOutput> output)
    {
        return std::make_shared<SwitchLight>(output, name);
    }

    void onButton(bool pressed);
    void onLamp(int brightness);
private:
    void toggleLight();

    std::string buttonLocation;
    std::string lampLocation;
    int mode = 0;
    int brightness = NAN_VALUE;
    bool current = false;
};

}  // namespace automation
