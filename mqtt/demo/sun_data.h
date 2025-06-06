#pragma once

#include <array>

struct SunData { 
    int sunrise; 
    int sunset;
};

int GetSunrise(int dayOfYear);
int GetSunset(int dayOfYear);

