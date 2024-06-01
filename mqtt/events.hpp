#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <ostream>
#include <variant>

enum class ValueType {
  TEMPERATURE,
  ELECTRICITY_PRICE,
  TIME,
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

    std::string name() {
        switch (type) {
            case ValueType::TEMPERATURE:
                return "Temperature";
            case ValueType::ELECTRICITY_PRICE:
                return "Electricity Price";
            case ValueType::TIME:
                return "Time";
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

private:
    ValueType type;
    std::variant<int, float> value;
    bool isInteger;
};

struct IValues {
    virtual int getInt(ValueType type) const = 0;
    virtual float getFloat(ValueType type) const = 0;
    virtual void set(ValueType type, int value) = 0;
    virtual bool set(ValueType type, float value) = 0;
};

struct Observer {
    virtual void onChange(const IValues& state) = 0;
};


struct Observable {
    void notify(const IValues& state) {
        for (auto observer : observers)
            observer->onChange(state);
    }

    void subscribe(Observer& observer) {
        observers.push_back(&observer);
    }

    void unsubscribe(Observer& observer) {
        observers.erase(
            remove(observers.begin(), observers.end(), &observer),
            observers.end()
        );
    }

private:
    std::vector<Observer*> observers;
};


struct Values : public Observable, public IValues {
    Values() {
        values[ValueType::TIME] = ValueItem(ValueType::TIME, 0);
        values[ValueType::TEMPERATURE] = ValueItem(ValueType::TEMPERATURE, 0.0f);
        values[ValueType::ELECTRICITY_PRICE] = ValueItem(ValueType::ELECTRICITY_PRICE, 0);
    }

    int getInt(ValueType type) const {
        return values.at(type).getInt();
        // return values[type].getInt();
    }

    float getFloat(ValueType type) const {
        return values.at(type).getFloat();
    }

    void set(ValueType type, int value) {
        values[type] = ValueItem(type, value);
    }

    bool set(ValueType type, float value) {
        float old = getFloat(type);
        if (old == value) {
            return false;
        }
        values[type] = ValueItem(type, value);
        notify(*this);
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Values& values) {
        for (auto& [type, item] : values.values) {
            os << item;
        }
        return os;
    }

private:
    std::map<ValueType, ValueItem> values;
};
