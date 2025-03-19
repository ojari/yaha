#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include "suntime.hpp"

const double PI = 3.14159265358979323846;
const double ZENITH = 90.833;  // Official zenith for sunrise/sunset

double degToRad(double deg) {
    return (deg * PI / 180.0);
}

double radToDeg(double rad) {
    return (rad * 180.0 / PI);
}

double SunTime::calculate(bool isSunrise) {
    // 1. Calculate the day of the year
    struct tm date = {0};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    time_t date_seconds = mktime(&date);
    struct tm *date_info = localtime(&date_seconds);
    int N = date_info->tm_yday + 1;

    // 2. Convert the longitude to hour value and calculate an approximate time
    double lngHour = longitude / 15.0;
    double t = isSunrise ? N + ((6 - lngHour) / 24) : N + ((18 - lngHour) / 24);

    // 3. Calculate the Sun's mean anomaly
    double M = (0.9856 * t) - 3.289;

    // 4. Calculate the Sun's true longitude
    double L = M + (1.916 * sin(degToRad(M))) + (0.020 * sin(degToRad(2 * M))) + 282.634;
    if (L > 360) L -= 360;
    if (L < 0) L += 360;

    // 5. Calculate the Sun's right ascension
    double RA = radToDeg(atan(0.91764 * tan(degToRad(L))));
    if (RA > 360) RA -= 360;
    if (RA < 0) RA += 360;

    // 6. Right ascension value needs to be in the same quadrant as L
    double Lquadrant  = (floor(L / 90)) * 90;
    double RAquadrant = (floor(RA / 90)) * 90;
    RA = RA + (Lquadrant - RAquadrant);

    // 7. Right ascension value needs to be converted into hours
    RA /= 15;

    // 8. Calculate the Sun's declination
    double sinDec = 0.39782 * sin(degToRad(L));
    double cosDec = cos(asin(sinDec));

    // 9. Calculate the Sun's local hour angle
    double cosH = (cos(degToRad(ZENITH)) - (sinDec * sin(degToRad(latitude)))) / (cosDec * cos(degToRad(latitude)));
    if (cosH > 1) return -1;   // The sun never rises on this location (on the specified date)
    if (cosH < -1) return -1;  // The sun never sets on this location (on the specified date)

    // 10. Finish calculating H and convert into hours
    double H = isSunrise ? 360 - radToDeg(acos(cosH)) : radToDeg(acos(cosH));
    H /= 15;

    // 11. Calculate local mean time of rising/setting
    double T = H + RA - (0.06571 * t) - 6.622;

    // 12. Adjust back to UTC
    double UT = T - lngHour;
    if (UT > 24) UT -= 24;
    if (UT < 0) UT += 24;

    return UT;
}

int SunTime::time2hm(double time) {
    int hours = static_cast<int>(time);
    int minutes = static_cast<int>((time - hours) * 60);

    // Populate the tm structure with the given UTC time
    std::tm utc_tm = {};
    utc_tm.tm_year = year - 1900;  // tm_year is years since 1900
    utc_tm.tm_mon = month - 1;     // tm_mon is months since January (0-11)
    utc_tm.tm_mday = day;
    utc_tm.tm_hour = hours;
    utc_tm.tm_min = minutes;
    utc_tm.tm_sec = 0;
    utc_tm.tm_isdst = -1;  // Let mktime determine if DST is in effect

    // Convert tm structure to epoch time (UTC)
    std::time_t utc_epoch_time = timegm(&utc_tm);

    // Convert epoch time to local time
    std::tm *local_tm = localtime(&utc_epoch_time);
    std::time_t local_epoch_time = mktime(local_tm);

    // return hours * 100 + minutes;
    return local_tm->tm_hour * 100 + local_tm->tm_min;
}

