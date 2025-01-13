#pragma once
#include <string>
#include "../common.hpp"

struct BaseData {
    long epoch;

    explicit BaseData(long time) : epoch(time) {}
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
    float uv = 0.0;
    float solarRadiation = 0.0;

    DataWeather(long time, float temp, float hum, float press, float windSpd, float windDir, float r) :
        BaseData(time),
        temperature(temp),
        humidity(hum),
        pressure(press),
        windSpeed(windSpd),
        windDirection(windDir),
        rain(r)
    {}

    void setSolarRadiation(float value) {
        solarRadiation = value;
    }

    void setUv(float value) {
        uv = value;
    }
};


struct DataElPrice : public BaseData {
    float price;

    DataElPrice(long time, float p) :
        BaseData(time),
        price(p)
    {}
};

struct DataHistory : public BaseData {
    EventId event;
    int val;

    DataHistory() :
        BaseData{0},
        event{EventId::UNKNOWN},
        val{0}
    {}
    DataHistory(long time, const EventId& ev, int v1) :
        BaseData{time},
        event{ev},
        val{v1}
    {}
};

struct DataInsertHistory {
    virtual ~DataInsertHistory() = default;

    virtual void insert(const DataHistory& data) = 0;
};
