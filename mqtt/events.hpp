#pragma once


struct EventTime {
    virtual void onTime(int time) = 0;
};

struct EventElPrice {
    virtual void onElPrice(int price) = 0;
};

struct EventTemperture {
    virtual void onTemperture(int temp) = 0;
};