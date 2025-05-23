#pragma once

#include "common.hpp"

class IActuator {
public:
    virtual ~IActuator() = default;

    virtual void set(std::string_view device, const std::string& value) = 0;
};


class Actuator : public IActuator, public IObserver {
public:
    explicit Actuator(IOutput* output) :
        output(output)
    {}
    void set(std::string_view device, const std::string& value) override;

    void onChange(const IEventData& event) override;

private:
    IOutput* output;
    int time {0};
};
