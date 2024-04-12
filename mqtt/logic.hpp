#pragma once
#include "events.hpp"


struct CarHeater : public EventTime, public EventTemperture {
    void onTime(int time) override;
    void onTemperture(int temp) override;
private:
    int leaveTime;
};

class StorageHeater : public EventTime, public EventElPrice {
    void onTime(int time) override;
    void onElPrice(int price) override;
};

class WaterHeater : public EventTime, public EventTemperture, public EventElPrice {
    void onTime(int time) override;
    void onTemperture(int temp) override;
    void onElPrice(int price) override;
};

class RoomHeater : public EventTime, public EventTemperture, public EventElPrice {
    void onTime(int time) override;
    void onTemperture(int temp) override;
    void onElPrice(int price) override;
};