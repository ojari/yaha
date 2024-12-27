#pragma once
#include <algorithm>
#include <ostream>
#include <variant>
#include "common.hpp"


class EventData : public IEventData {
public:
    EventData () :
        type(EventId::UNKNOWN),
        value(0),
        isInteger(true)
    {}

    EventData(EventId type, int value) : 
        type(type),
        value(value),
        isInteger(true)
    {}

    EventData(EventId type, float value) :
        type(type),
        value(value),
        isInteger(false)
    {}

    EventId id() const override {
        return type;
    }

    int getInt(int default_val = -1) const override {
        if (!isInteger) {
            return default_val;
        }
        return std::get<int>(value);
    }

    float getFloat() const override {
        return std::get<float>(value);
    }

    bool isInt() const override {
        return isInteger;
    }

    std::string name() const override {
        return event2str(type);
    }

    friend std::ostream& operator<<(std::ostream& os, const EventData& item) {
        os << "Type: " << static_cast<int>(item.id()) << std::endl;
        if (item.isInt()) {
            os << "Value: " << item.getInt() << std::endl;
        } else {
            os << "Value: " << item.getFloat() << std::endl;
        }
        return os;
    }

    void setv(IEventData &item) override {
        type = item.id();
        if (item.isInt()) {
            value = item.getInt();
            isInteger = true;
        } else {
            value = item.getFloat();
            isInteger = false;
        }
    }

    void set(int aValue) override {
        this->value = aValue;
        isInteger = true;
    }

    void set(float aValue) override {
        this->value = aValue;
        isInteger = false;
    }

private:
    EventId type;
    std::variant<int, float> value;
    bool isInteger;
};


