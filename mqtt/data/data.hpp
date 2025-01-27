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
    float temperature = 0.0;
    float humidity = 0.0;
    float pressure = 0.0;
    float windSpeed = 0.0;
    float windDirection = 0.0;
    float rain = 0.0;
    float uv = 0.0;
    float solarRadiation = 0.0;

    DataWeather(long time) :
        BaseData(time)
    {}

    DataWeather(long time, float temp, float hum, float press, float windSpd, float windDir, float r) :
        BaseData(time),
        temperature(temp),
        humidity(hum),
        pressure(press),
        windSpeed(windSpd),
        windDirection(windDir),
        rain(r)
    {}

    void setTemperature(float value) {
        temperature = value;
    }

    void setHumidity(float value) {
        humidity = value;
    }

    void setPressure(float value) {
        pressure = value;
    }

    void setWindSpeed(float value) {
        windSpeed = value;
    }

    void setWindDirection(float value) {
        windDirection = value;
    }

    void setRain(float value) {
        rain = value;
    }

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
