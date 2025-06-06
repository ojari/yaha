#pragma once

#include <string>
#include <string_view>

#ifdef DEBUG_TIME
    const int FAST_TIMER_INTERVAL = 10;
    const int SLOW_TIMER_INTERVAL = 500;
#else
    const int FAST_TIMER_INTERVAL = 60000;
    const int SLOW_TIMER_INTERVAL = 5*60000;
#endif

constexpr std::string_view MQTT_TOPIC = "zigbee2mqtt";