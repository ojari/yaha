#pragma once

enum EventType {
    TIME,
    TEMPERATURE,
    EL_PRICE
};

struct Event {
    virtual void onEvent(EventType type, int arg) = 0;
};
