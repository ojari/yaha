#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <ostream>
#include <variant>
#include "common.hpp"


class ValueItem : public IValueItem {
public:
    ValueItem () : type(ValueType::UNKNOWN), value(0), isInteger(true) {}
    ValueItem(ValueType type, int value) : type(type), value(value), isInteger(true) {}
    ValueItem(ValueType type, float value) : type(type), value(value), isInteger(false) {}

    ValueType getType() const override {
        return type;
    }

    int getInt() const override {
        return std::get<int>(value);
    }

    float getFloat() const override {
        return std::get<float>(value);
    }

    bool isInt() const override {
        return isInteger;
    }

    std::string name() const override {
        switch (type) {
            case ValueType::TEMPERATURE:
                return "Temperature";
            case ValueType::ELECTRICITY_PRICE:
                return "Electricity Price";
            case ValueType::TIME:
                return "Time";
            case ValueType::WEEKDAY:
                return "Weekday";
            case ValueType::SUNRISE:
                return "Sun Rise";
            case ValueType::SUNSET:
                return "Sun Set";
            default:
                return "Unknown";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ValueItem& item) {
        os << "Type: " << static_cast<int>(item.getType()) << std::endl;
        if (item.isInt()) {
            os << "Value: " << item.getInt() << std::endl;
        } else {
            os << "Value: " << item.getFloat() << std::endl;
        }
        return os;
    }

    void setv(IValueItem &item) override {
        type = item.getType();
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
    ValueType type;
    std::variant<int, float> value;
    bool isInteger;
};


