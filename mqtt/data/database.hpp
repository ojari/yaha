#pragma once
#include <sqlite3.h>
#include "data.hpp"
#include "common.hpp"
#include <vector>
#include <memory>


class DataTemperatureORM : public BaseORM {
public:
    DataTemperatureORM(sqlite3* db) :
        BaseORM(db) 
    {}

    const char* sqlCreateTable() override {
        return "CREATE TABLE IF NOT EXISTS Temperature ("
               "epoch INTEGER PRIMARY KEY,"
               "temperature REAL,"
               "humidity REAL);";
    }

    void insert(const DataTemperature& data) {
        auto sql = SqlInsert(db, "INSERT INTO Temperature (epoch, temperature, humidity) VALUES (?, ?, ?);");
        sql.add(data.epoch);
        sql.add(data.temperature);
        sql.add(data.humidity);
    }

    SqlIterator<DataTemperature> begin() {
        const char* sql = "SELECT epoch, temperature, humidity FROM Temperature;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        return SqlIterator<DataTemperature>(stmt, [](sqlite3_stmt* stmt) {
            long epoch = sqlite3_column_int64(stmt, 0);
            float temperature = sqlite3_column_double(stmt, 1);
            float humidity = sqlite3_column_double(stmt, 2);
            return DataTemperature(epoch, temperature, humidity);
        });
    }

    SqlIterator<DataTemperature> end() {
        return SqlIterator<DataTemperature>(nullptr, {});
    }

    std::vector<DataTemperature> getAll() {
        const char* sql = "SELECT epoch, temperature, humidity FROM Temperature;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

        std::vector<DataTemperature> data;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long epoch = sqlite3_column_int64(stmt, 0);
            float temperature = sqlite3_column_double(stmt, 1);
            float humidity = sqlite3_column_double(stmt, 2);
            data.push_back(DataTemperature(epoch, temperature, humidity));
        }

        sqlite3_finalize(stmt);
        return data;
    }
};

class DataWeatherORM : public BaseORM {
public:
    DataWeatherORM(sqlite3* db) :
        BaseORM(db)
    {}

    const char* sqlCreateTable() override {
        return "CREATE TABLE IF NOT EXISTS Weather ("
               "epoch INTEGER PRIMARY KEY,"
               "temperature REAL,"
               "humidity REAL,"
               "pressure REAL,"
               "windSpeed REAL,"
               "windDirection REAL,"
               "rain REAL,"
               "uv REAL,"
               "solarRadiation REAL);";
    }

    void insert(const DataWeather& data) {
        auto sql = SqlInsert(db, "INSERT INTO Weather (epoch, temperature, humidity, pressure, windSpeed, windDirection, rain, uv, solarRadiation) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
        sql.add(data.epoch);
        sql.add(data.temperature);
        sql.add(data.humidity);
        sql.add(data.pressure);
        sql.add(data.windSpeed);
        sql.add(data.windDirection);
        sql.add(data.rain);
        sql.add(data.uv);
        sql.add(data.solarRadiation);
    }

    std::vector<DataWeather> getAll() {
        const char* sql = "SELECT epoch, temperature, humidity, pressure, windSpeed, windDirection, rain, uv, solarRadiation FROM Weather;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

        std::vector<DataWeather> data;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long epoch = sqlite3_column_int64(stmt, 0);
            float temperature = sqlite3_column_double(stmt, 1);
            float humidity = sqlite3_column_double(stmt, 2);
            float pressure = sqlite3_column_double(stmt, 3);
            float windSpeed = sqlite3_column_double(stmt, 4);
            float windDirection = sqlite3_column_double(stmt, 5);
            float rain = sqlite3_column_double(stmt, 6);
            float uv = sqlite3_column_double(stmt, 7);
            float solarRadiation = sqlite3_column_double(stmt, 8);
            data.push_back(DataWeather(epoch, temperature, humidity, pressure, windSpeed, windDirection, rain, uv, solarRadiation));
        }

        sqlite3_finalize(stmt);
        return data;
    }
};

class DataElPriceORM : public BaseORM {
public:
    DataElPriceORM(sqlite3* db) : 
        BaseORM(db)
    {}

    const char* sqlCreateTable() override {
        return "CREATE TABLE IF NOT EXISTS ElPrice ("
               "epoch INTEGER PRIMARY KEY,"
               "price REAL);";
    }

    void insert(const DataElPrice& data) {
        auto sql = SqlInsert(db, "INSERT INTO ElPrice (epoch, price) VALUES (?, ?);");
        sql.add(data.epoch);
        sql.add(data.price);
    }

    std::vector<DataElPrice> getAll() {
        const char* sql = "SELECT epoch, price FROM ElPrice;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

        std::vector<DataElPrice> data;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long epoch = sqlite3_column_int64(stmt, 0);
            float price = sqlite3_column_double(stmt, 1);
            data.push_back(DataElPrice(epoch, price));
        }

        sqlite3_finalize(stmt);
        return data;
    }
};

class DataHistoryORM : public BaseORM, public DataInsertHistory {
public:
    DataHistoryORM(sqlite3* db) :
        BaseORM(db)
    {}

    const char* sqlCreateTable() override {
        return "CREATE TABLE IF NOT EXISTS History ("
               "epoch INTEGER PRIMARY KEY,"
               "device TEXT,"
               "type INTEGER,"
               "val1 INTEGER,"
               "val2 INTEGER,"
               "val3 INTEGER);";
    }

    void insert(const DataHistory& data) override {
        auto sql = SqlInsert(db, "INSERT INTO History (epoch, device, type, val1, val2, val3) VALUES (?, ?, ?, ?, ?, ?);");
        sql.add(data.epoch);
        sql.add(data.device);
        sql.add(data.type);
        sql.add(data.val1);
        sql.add(data.val2);
        sql.add(data.val3);
    }

    std::vector<DataHistory> getAll() {
        const char* sql = "SELECT epoch, device, type, val1, val2, val3 FROM History;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

        std::vector<DataHistory> data;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long epoch = sqlite3_column_int64(stmt, 0);
            std::string device = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            DataType type = static_cast<DataType>(sqlite3_column_int(stmt, 2));
            int val1 = sqlite3_column_int(stmt, 3);
            int val2 = sqlite3_column_int(stmt, 4);
            int val3 = sqlite3_column_int(stmt, 5);
            data.push_back(DataHistory(epoch, device, type, val1, val2, val3));
        }

        sqlite3_finalize(stmt);
        return data;
    }
};


class Database {
public:
    Database(sqlite3* db) : 
        temperature(db),
        weather(db),
        elPrice(db),
        history(db),
        db(db)
    {
    }

    ~Database() {
        sqlite3_close(db);
    }

    void createTables() {
        createTable(temperature.sqlCreateTable());
        createTable(weather.sqlCreateTable());
        createTable(elPrice.sqlCreateTable());
        createTable(history.sqlCreateTable());
    }
    
    void insert(const DataTemperature& data) {
        temperature.insert(data);
    }
    void insert(const DataWeather& data) {
        weather.insert(data);
    }
    void insert(const DataElPrice& data) {
        elPrice.insert(data);
    }
    void insert(const DataHistory& data) {
        history.insert(data);
    }

    DataTemperatureORM temperature;
    DataWeatherORM weather;
    DataElPriceORM elPrice;
    DataHistoryORM history;

private:
    void createTable(const char* sql);

    sqlite3* db;
};


std::unique_ptr<Database> createDatabase(const std::string& filename);
