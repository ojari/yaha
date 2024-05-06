#pragma once

#include <string>
#include "common.hpp"

class IActuator {
public:
    virtual void set(std::string &device, bool value) = 0;
};


class Actuator : public IActuator {
public:
    Actuator(IOutput& output) : 
        output(output) 
    {}
    void set(std::string &device, bool value) override;
    
private:
    IOutput& output;
};