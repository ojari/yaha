#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <ostream>
#include <variant>
#include "common.hpp"

enum class ValueType {
  TEMPERATURE,
  ELECTRICITY_PRICE,
  TIME,
  WEEKDAY,
  SUNRISE,
  SUNSET,
  UNKNOWN
};


class ValueItem {
public:
    ValueItem () : type(ValueType::UNKNOWN), value(0), isInteger(true) {}
    ValueItem(ValueType type, int value) : type(type), value(value), isInteger(true) {}
    ValueItem(ValueType type, float value) : type(type), value(value), isInteger(false) {}

    ValueType getType() const {
        return type;
    }

    int getInt() const {
        return std::get<int>(value);
    }

    float getFloat() const {
        return std::get<float>(value);
    }

    bool isInt() const {
        return isInteger;
    }

    std::string name() const {
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

    void set(ValueItem item) {
        type = item.getType();
        if (item.isInt()) {
            value = item.getInt();
            isInteger = true;
        } else {
            value = item.getFloat();
            isInteger = false;
        }
    }

    void set(int aValue) {
        this->value = aValue;
        isInteger = true;
    }

    void set(float aValue) {
        this->value = aValue;
        isInteger = false;
    }

private:
    ValueType type;
    std::variant<int, float> value;
    bool isInteger;
};

struct IObserver {
    virtual ~IObserver() = default;

    virtual void onChange(const ValueItem& value) = 0;
};

struct Observable {
    void notify(const ValueItem& value) const {
        for (auto observer : observers)
            observer->onChange(value);
    }

    void subscribe(IObserver& observer) {
        observers.push_back(&observer);
    }

    void unsubscribe(IObserver& observer) {
        observers.erase(
            remove(observers.begin(), observers.end(), &observer),
            observers.end()
        );
    }

private:
    std::vector<IObserver*> observers;
};


enum class ETask {
    TEMPERATURE,
    TIME,
    PRICE,
    MQTT
};

struct ITaskManager {
    virtual ~ITaskManager() = default;

    virtual void subscribe(ETask taskId, IObserver& observer) = 0;

    virtual void execute() = 0;
};

