#pragma once
#include <sqlite3.h>
#include <string>
#include "data.hpp"

struct BaseData {
    long epoch;

    BaseData(long time) : epoch(time) {}
};

struct DataTemperature : public BaseData {
    float temperature;
    float humidity;

    DataTemperature(long time, float temp, float hum) :
        BaseData(time),
        temperature(temp), 
        humidity(hum)
    {}
};

struct DataWeather : public BaseData {
    float temperature;
    float humidity;
    float pressure;
    float windSpeed;
    float windDirection;
    float rain;
    float uv;
    float solarRadiation;

    DataWeather(long time, float temp, float hum, float press, float windSpd, float windDir, float r, float uv, float solarRad) :
        BaseData(time),
        temperature(temp),
        humidity(hum),
        pressure(press),
        windSpeed(windSpd),
        windDirection(windDir),
        rain(r),
        uv(uv),
        solarRadiation(solarRad)
    {}
};


struct DataElPrice : public BaseData {
    float price;

    DataElPrice(long time, float p) :
        BaseData(time),
        price(p)
    {}
};


struct DataHistory : public BaseData {
    std::string device;
    int state;

    DataHistory(long time, const std::string& dev, int st) :
        BaseData(time),
        device(dev),
        state(st)
    {}
};
