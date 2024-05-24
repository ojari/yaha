#pragma once
#include <vector>
#include <string>
#include <variant>
#include <stdexcept> // Add this line

#include "config.hpp"

class DataValue {
public:
    DataValue(const std::string& name, int value) : name(name), value(value) {}
    DataValue(const std::string& name, double value) : name(name), value(value) {}
    DataValue(const std::string& name, const std::string& value) : name(name), value(value) {}

    const std::string& getName() const {
        return name;
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
    std::variant<int, double, std::string> value;
};

class DataHeader {
public:

    template <typename T>
    void setValue(const std::string& name, T value) {
        for (DataValue& data : values) {
            if (data.getName() == name) {
                data.setValue(value);
                return;
            }
        }
        throw std::runtime_error("Value not found");
    }

    template <typename T>
    T getValue(const std::string& name) const {
        for (const DataValue& value : values) {
            if (value.getName() == name) {
                return value.getValue<T>();
            }
        }
        throw std::runtime_error("Value not found");
    }

    std::string getNames() {
        std::string names;
        for (const DataValue& value : values) {
            names += value.getName() + " ";
        }
        return names;
    }

protected:
    void add(const DataValue& value) {
        values.push_back(value);
    }

private:
    std::vector<DataValue> values;
};

template <typename T>
struct IDataTable {
    virtual ~IDataTable() {}

    virtual void set(const T& data) = 0;
};

class TableConfigDevice : public DataHeader, IDataTable<ConfigDevice> {
public:
    TableConfigDevice() {
        add(DataValue("name", ""));
        add(DataValue("type", ""));
    }

    void set(const ConfigDevice& config) override {
        setValue("name", config.name);
        setValue("type", config.type);
    }

    ConfigDevice getConfig() const {
        return ConfigDevice(getValue<std::string>("name"), getValue<std::string>("type"));
    }
};

class TableConfigController : public DataHeader, IDataTable<ConfigController> {
public:
    TableConfigController() {
        add(DataValue("name", ""));
        add(DataValue("type", ""));
        add(DataValue("actuator", ""));
        add(DataValue("time1", 0));
        add(DataValue("time2", 0));
    }

    void set(const ConfigController& config) override {
        setValue("name", config.name);
        setValue("type", config.type);
        setValue("actuator", config.actuator);
        setValue("time1", config.time1);
        setValue("time2", config.time2);
    }

    ConfigController getConfig() const {
        return ConfigController(
            getValue<std::string>("name"),
            getValue<std::string>("type"),
            getValue<std::string>("actuator"),
            getValue<int>("time1"),
            getValue<int>("time2")
        );
    }
};


class DataTable {
public:
    DataTable() {
        tables.push_back(std::make_unique<TableConfigDevice>());
        tables.push_back(std::make_unique<TableConfigController>());
    }

    template <typename T>
    T getValue(int row, const std::string& name) const {
        return tables[row]->getValue<T>(name);
    }

    template <typename T>
    void setValue(int row, const std::string& name, T value) {
        tables[row]->setValue(name, value);
    }
private:
    std::vector<std::unique_ptr<DataHeader>> tables;
};