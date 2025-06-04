#pragma once
#include <vector>
#include <stdexcept>
#include "config.hpp"
#include "data.hpp"
#include "datasource.hpp"

namespace data {

class DataHeader : public IDataHeader {
public:
    explicit DataHeader(const std::string& tableName) :
        tableName(tableName)
    {}

    ~DataHeader() override = default;

    template <typename T>
    void setValue(std::string_view name, T value) {
        for (DataValue& data : values) {
            if (data.getName() == name) {
                data.setValue(value);
                return;
            }
        }
        throw std::runtime_error("Value not found");
    }

    template <typename T>
    T getValue(std::string_view name) const {
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

    const std::string& getTableName() const override {
        return tableName;
    }

protected:
    void add(const DataValue& value) {
        values.push_back(value);
    }

private:
    std::vector<DataValue> values;
    std::string tableName;
};

class TableConfigDevice : public DataHeader {
public:
    TableConfigDevice() : DataHeader("device") {
        add(DataValue("name", ""));
        add(DataValue("type", ""));
    }
};
extern void dataToHeader(DataHeader& header, const ConfigDevice& config);
extern void dataFromHeader(const DataHeader& header, ConfigDevice& config);

class TableConfigController : public DataHeader {
public:
    TableConfigController() : DataHeader("controller"){
        add(DataValue("name", ""));
        add(DataValue("type", ""));
        add(DataValue("actuator", ""));
        add(DataValue("time1", 0));
        add(DataValue("time2", 0));
    }
};
extern void dataToHeader(DataHeader& header, const ConfigController& config);
extern void dataFromHeader(const DataHeader& header, ConfigController& config);


class TableTemperature : public DataHeader {
public:
    TableTemperature() : DataHeader("Temperature") {
        add(DataValue("epoch", 0L));
        add(DataValue("temperature", 0.0f));
        add(DataValue("humidity", 0.0f));
    }
};
extern void dataToHeader(DataHeader& header, const DataTemperature& data);
extern void dataFromHeader(const DataHeader& header, DataTemperature& data);

class TableWeather : public DataHeader {
public:
    explicit TableWeather(long epoch = 0) : DataHeader("Weather") {
        add(DataValue("epoch", epoch));
        add(DataValue("temperature", 0.0f));
        add(DataValue("humidity", 0.0f));
        add(DataValue("pressure", 0.0f));
        add(DataValue("windSpeed", 0.0f));
        add(DataValue("windDirection", 0.0f));
        add(DataValue("rain", 0.0f));
        add(DataValue("uv", 0.0f));
        add(DataValue("solarRadiation", 0.0f));
    }
};
extern void dataToHeader(DataHeader& header, const DataWeather& data);
extern void dataFromHeader(const DataHeader& header, DataWeather& data);

class TableElPrice : public DataHeader {
public:
    TableElPrice() : DataHeader("ElPrice") {
        add(DataValue("epoch", 0L));
        add(DataValue("price", 0.0f));
    }
};
extern void dataToHeader(DataHeader& header, const DataElPrice& data);
extern void dataFromHeader(const DataHeader& header, DataElPrice& data);

class TableHistory : public DataHeader {
public:
    TableHistory() : DataHeader("History") {
        add(DataValue("epoch", 0L));
        add(DataValue("event", 0));
        add(DataValue("val", 0));
    }
};
extern void dataToHeader(DataHeader& header, const DataHistory& data);
extern void dataFromHeader(const DataHeader& header, DataHistory& data);

class DataTable {
public:
    DataTable() {
        tables.push_back(std::make_unique<TableConfigDevice>());
        tables.push_back(std::make_unique<TableConfigController>());
        tables.push_back(std::make_unique<TableTemperature>());
        tables.push_back(std::make_unique<TableWeather>());
        tables.push_back(std::make_unique<TableElPrice>());
        tables.push_back(std::make_unique<TableHistory>());
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

} // namespace data