#pragma once
#include "events.hpp"

struct DebugOutput : public Observer {
    void onChange(const Values& state) override;
};

struct Lights : public Observer {    
    Lights(int onTime, int offTime) :
        onTime(onTime),
        offTime(offTime)
    {}
    void onChange(const Values& state) override;
private:
    int onTime;
    int offTime;
};

struct CarHeater : public Observer {
    CarHeater(int leaveTime) :
        leaveTime(leaveTime)
    {}
    void onChange(const Values& state) override;
private:
    int calculateDuration(const Values& state);
    int leaveTime;
};

class StorageHeater : public Observer {
    void onChange(const Values& state) override;
private:
    int calculateStartTime(const Values& state);
    int calculateEndTime(const Values& state);
};

class WaterHeater : public Observer {
    void onChange(const Values& state) override;
};

class RoomHeater : public Observer {
    void onChange(const Values& state) override;
};