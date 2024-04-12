#pragma once
#include "events.hpp"

struct Lights : public Event {
    void onEvent(EventType type, int arg) override;
private:
    int onTime;
    int offTime;
};

struct CarHeater : public Event {
    void onEvent(EventType type, int arg) override;
private:
    int leaveTime;
};

class StorageHeater : public Event {
    void onEvent(EventType type, int arg) override;
};

class WaterHeater : public Event {
    void onEvent(EventType type, int arg) override;
};

class RoomHeater : public Event {
    void onEvent(EventType type, int arg) override;
};