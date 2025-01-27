#pragma once
#include <string>
#include <variant>
#include <vector>
#include <stdexcept>

enum class DataValueType {
    INT,
    LONG,
    DOUBLE,
    STRING
};

class DataValue {
public:
    DataValue(const std::string& name, int value) : name(name), value(value) {}
    DataValue(const std::string& name, long value) : name(name), value(value) {}
    DataValue(const std::string& name, float value) : name(name), value(value) {}
    DataValue(const std::string& name, const std::string& value) : name(name), value(value) {}

    const std::string& getName() const {
        return name;
    }

    DataValueType getType() const {
        if (std::holds_alternative<int>(value)) {
            return DataValueType::INT;
        }
        else if (std::holds_alternative<long>(value)) {
            return DataValueType::LONG;
        }
        else if (std::holds_alternative<float>(value)) {
            return DataValueType::DOUBLE;
        }
        else if (std::holds_alternative<std::string>(value)) {
            return DataValueType::STRING;
        }
        throw std::runtime_error("Invalid data type");
    }

    template <typename T>
    T getValue() const {
        return std::get<T>(value);
    }

    template <typename T>
    void setValue(T& newValue) {
        this->value.emplace<T>(newValue);
    }
private:
    std::string name;
    std::variant<int, long, float, std::string> value;
};


struct IDataHeader {
    using iterator = std::vector<DataValue>::iterator;
    virtual ~IDataHeader() = default;

    virtual iterator begin() = 0;
    virtual iterator end() = 0;

    virtual const std::string& getTableName() const = 0;
};
