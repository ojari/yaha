#pragma once

struct SunTime {
    SunTime() :
        latitude(0.0),
        longitude(0.0)
    {}

    SunTime(double latitude, double longitude) :
        latitude(latitude),
        longitude(longitude)
    {}

    void setLocation(double latitude, double longitude) {
        this->latitude = latitude;
        this->longitude = longitude;
    }

    void setYear(int year) {
        this->year = year;
    }

    void setDate(int date, int month, int day) {
        this->year = year;
        this->month = month;
        this->day = day;
    }

    double calculate(bool isSunrise);

    int time2hm(double time);

private:
    double latitude;
    double longitude;

    int year = 2026;
    int month = 2;
    int day = 2;
};
