#pragma once

#ifdef DEBUG_TIME
    const int FAST_TIMER_INTERVAL = 20;
    const int SLOW_TIMER_INTERVAL = 1000;
#else
    const int FAST_TIMER_INTERVAL = 60000;
    const int SLOW_TIMER_INTERVAL = 5*60000;
#endif
