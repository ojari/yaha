#pragma once
#include <vector>
#include <stdexcept> // Add this line

#include "config.hpp"
#include "data.hpp"
#include "datasource.hpp"

class DataHeader : public IDataHeader {
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

    using iterator = std::vector<DataValue>::iterator;

    iterator begin() override {
        return values.begin();
    }

    iterator end() override {
        return values.end();
    }

protected:
    void add(const DataValue& value) {
        values.push_back(value);
    }

private:
    std::vector<DataValue> values;
};

class TableConfigDevice : public DataHeader, public IDataTable<ConfigDevice> {
public:
    TableConfigDevice() : IDataTable("device") {
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

class TableConfigController : public DataHeader, public IDataTable<ConfigController> {
public:
    TableConfigController() : IDataTable("controller"){
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


class TableTemperature : public DataHeader, public IDataTable<DataTemperature> {
public:
    TableTemperature() : IDataTable("Temperature") {
        add(DataValue("epoch", 0L));
        add(DataValue("temperature", 0.0f));
        add(DataValue("humidity", 0.0f));
    }

    void set(const DataTemperature& data) override {
        setValue("epoch", data.epoch);
        setValue("temperature", data.temperature);
        setValue("humidity", data.humidity);
    }
    DataTemperature get() const {
        return DataTemperature(
            getValue<int>("epoch"),
            getValue<float>("temperature"),
            getValue<float>("humidity")
        );
    }
};


class TableWeather : public DataHeader, public IDataTable<DataWeather> {
public:
    TableWeather() : IDataTable("Weather") {
        add(DataValue("epoch", 0L));
        add(DataValue("temperature", 0.0f));
        add(DataValue("humidity", 0.0f));
        add(DataValue("pressure", 0.0f));
        add(DataValue("windSpeed", 0.0f));
        add(DataValue("windDirection", 0.0f));
        add(DataValue("rain", 0.0f));
        add(DataValue("uv", 0.0f));
        add(DataValue("solarRadiation", 0.0f));
    }

    void set(const DataWeather& data) override {
        setValue("epoch", data.epoch);
        setValue("temperature", data.temperature);
        setValue("humidity", data.humidity);
        setValue("pressure", data.pressure);
        setValue("windSpeed", data.windSpeed);
        setValue("windDirection", data.windDirection);
        setValue("rain", data.rain);
        setValue("uv", data.uv);
        setValue("solarRadiation", data.solarRadiation);
    }
    DataWeather get() const {
        return DataWeather(
            getValue<int>("epoch"),
            getValue<float>("temperature"),
            getValue<float>("humidity"),
            getValue<float>("pressure"),
            getValue<float>("windSpeed"),
            getValue<float>("windDirection"),
            getValue<float>("rain"),
            getValue<float>("uv"),
            getValue<float>("solarRadiation")
        );
    }
};


class TableElPrice : public DataHeader, public IDataTable<DataElPrice> {
public:
    TableElPrice() : IDataTable("ElPrice") {
        add(DataValue("epoch", 0L));
        add(DataValue("price", 0.0f));
    }

    void set(const DataElPrice& data) override {
        setValue("epoch", data.epoch);
        setValue("price", data.price);
    }
    DataElPrice get() const {
        return DataElPrice(
            getValue<int>("epoch"),
            getValue<float>("price")
        );
    }
};


class TableHistory : public DataHeader, public IDataTable<DataHistory> {
public:
    TableHistory() : IDataTable("History") {
        add(DataValue("epoch", 0L));
        add(DataValue("device", ""));
        add(DataValue("type", 0));
        add(DataValue("val1", 0));
        add(DataValue("val2", 0));
        add(DataValue("val3", 0));
    }

    void set(const DataHistory& data) override {
        setValue("epoch", data.epoch);
        setValue("device", data.device);
        setValue("type", static_cast<int>(data.type));
        setValue("val1", data.val1);
        setValue("val2", data.val2);
        setValue("val3", data.val3);
    }
    DataHistory get() const {
        return DataHistory(
            getValue<long>("epoch"),
            getValue<std::string>("device"),
            static_cast<DataType>(getValue<int>("type")),
            getValue<int>("val1"),
            getValue<int>("val2"),
            getValue<int>("val3")
        );
    }
};

class DataTable {
public:
    DataTable() {
        tables.push_back(std::make_unique<TableConfigDevice>());
        tables.push_back(std::make_unique<TableConfigController>());
        tables.push_back(std::make_unique<TableTemperature>());
        tables.push_back(std::make_unique<TableWeather>());
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