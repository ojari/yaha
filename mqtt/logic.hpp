#pragma once
#include "events.hpp"
#include <string>

struct DebugOutput : public Observer {
    void onChange(const Values& state) override;
};

struct BooleanOutput {
    BooleanOutput(const std::string& name) :
        state(false),
        name(name)
    {}

    bool isOn() const {
        return state;
    }
protected:
    void set(bool value);
private:
    bool state;
    std::string name;
};

struct Lights : public BooleanOutput, public Observer {    
    Lights(int onTime, int offTime) :
        onTime(onTime),
        offTime(offTime),
        BooleanOutput("Lights")
    {}
    void onChange(const Values& vars) override;
private:
    int onTime;
    int offTime;
};

struct CarHeater : public BooleanOutput, public Observer {
    CarHeater(int leaveTime) :
        leaveTime(leaveTime),
        BooleanOutput("CarHeater")
    {}
    void onChange(const Values& state) override;
private:
    int calculateDuration(const Values& state);
    int leaveTime;
    bool state;
};

class StorageHeater : public BooleanOutput, public Observer {
public:
    StorageHeater() :
        BooleanOutput("StorageHeater")
    {}
    void onChange(const Values& state) override;
private:
    int calculateStartTime(const Values& state);
    int calculateEndTime(const Values& state);
};

class WaterHeater : public BooleanOutput, public Observer {
public:
    WaterHeater() :
        BooleanOutput("WaterHeater")
    {}
    void onChange(const Values& state) override;
};

class RoomHeater : public BooleanOutput, public Observer {
public:
    RoomHeater() :
        BooleanOutput("RoomHeater")
    {}
    void onChange(const Values& state) override;
};