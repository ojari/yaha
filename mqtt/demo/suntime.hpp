#pragma once

class SunTime {
public:
    SunTime(double latitude, double longitude) : 
        latitude(latitude),
        longitude(longitude)
    {}

    void set(int year, int month, int day) {
        this->year = year;
        this->month = month;
        this->day = day;
    }

    double calculate(bool isSunrise);

    int time2hm(double time);

private:
    double latitude;
    double longitude;

    int year;
    int month;
    int day;
};