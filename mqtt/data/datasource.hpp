#pragma once
#include <string>
#include <variant>
#include <vector>
#include <stdexcept>

//#include "database.hpp"

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
        } else if (std::holds_alternative<long>(value)) {
            return DataValueType::LONG;
        } else if (std::holds_alternative<float>(value)) {
            return DataValueType::DOUBLE;
        } else if (std::holds_alternative<std::string>(value)) {
            return DataValueType::STRING;
        }
        throw std::runtime_error("Invalid data type");
    }

    template <typename T>
    T getValue() const {
        return std::get<T>(value);
    }

    template <typename T>
    void setValue(T& value) {
        this->value.emplace<T>(value);
    }
private:
    std::string name;
    std::variant<int, long, float, std::string> value;
};



template <typename T>
struct IDataTable {
    IDataTable(const std::string& tableName) : 
        tableName(tableName)
    {}
    virtual ~IDataTable() {}

    const std::string& getTableName() const {
        return tableName;
    }

    virtual void set(const T& data) = 0;

protected:
    std::string tableName;
};


struct IDataHeader {
    using iterator = std::vector<DataValue>::iterator;

    virtual iterator begin() = 0;
    virtual iterator end() = 0;
};
