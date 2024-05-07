#pragma once
#include <string>

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

enum DataType {
    LIGHT = 0,
    SWITCH = 1,
    TEMPERATURE = 2
};

struct DataHistory : public BaseData {
    std::string device;
    DataType type;
    int val1;
    int val2;
    int val3;

    DataHistory() :
        BaseData{0},
        device{"unknown"},
        type{DataType::LIGHT},
        val1{0},
        val2{0},
        val3{0}
    {}
    DataHistory(long time, const std::string& dev, DataType type, int v1, int v2, int v3) :
        BaseData{time},
        device{dev},
        type{type},
        val1{v1},
        val2{v2},
        val3{v3}
    {}
};

struct DataInsertHistory {
    virtual void insert(const DataHistory& data) = 0;
};
