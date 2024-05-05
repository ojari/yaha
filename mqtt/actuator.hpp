#pragma once

#include <string>

class IActuator {
public:
    virtual void set(std::string &device, bool value) = 0;
};


class Actuator : public IActuator {
public:
    void set(std::string &device, bool value) override;
};