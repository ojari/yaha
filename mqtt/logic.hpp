#pragma once
#include "events.hpp"

struct Lights : public Observer {    
    void onChange(const Values& state) override;
private:
    int onTime;
    int offTime;
};

struct CarHeater : public Observer {
    void onChange(const Values& state) override;
private:
    int calculateStartTime(const Values& state);
    int leaveTime;
};

class StorageHeater : public Observer {
    void onChange(const Values& state) override;
};

class WaterHeater : public Observer {
    void onChange(const Values& state) override;
};

class RoomHeater : public Observer {
    void onChange(const Values& state) override;
};