#pragma once

#include "common.hpp"

class IActuator {
public:
    virtual ~IActuator() = default;

    virtual void set(std::string_view device, bool value) = 0;
};


class Actuator : public IActuator, public IObserver {
public:
    explicit Actuator(const IOutput* output) : 
        output(output) 
    {}
    void set(std::string_view device, bool value) override;

    void onChange(const IEventData& event) override;
    
private:
    const IOutput* output;
    int time {0};
};