#pragma once
#include <sqlite3.h>
#include <string>
#include "data.hpp"

struct BaseData {
    long epoch;

    virtual std::string insert() const = 0;
};

struct DataTemperature : public BaseData {
    float temperature;
    float humidity;

    static std::string createTable() {
        return "CREATE TABLE IF NOT EXISTS DataTemperature ("
               "epoch INTEGER PRIMARY KEY,"
               "temperature REAL,"
               "humidity REAL);";
    }

    std::string insert() const override {
        return "INSERT INTO DataTemperature (epoch, temperature, humidity) VALUES ("
               + std::to_string(epoch) + ","
               + std::to_string(temperature) + ","
               + std::to_string(humidity) + ");";
    }
};

struct DataWeather : public BaseData {
    float temperature;
    float humidity;
    float pressure;
    float windSpeed;
    float windDirection;
    float rain;
    float uv;
    float solarRadiation;

    static std::string createTable() {
        return "CREATE TABLE IF NOT EXISTS DataWeather ("
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

    std::string insert() const override {
        return "INSERT INTO DataWeather (epoch, temperature, humidity, pressure, windSpeed, windDirection, rain, uv, solarRadiation) VALUES ("
               + std::to_string(epoch) + ","
               + std::to_string(temperature) + ","
               + std::to_string(humidity) + ","
               + std::to_string(pressure) + ","
               + std::to_string(windSpeed) + ","
               + std::to_string(windDirection) + ","
               + std::to_string(rain) + ","
               + std::to_string(uv) + ","
               + std::to_string(solarRadiation) + ");";
    }
};

struct DataElPrice : public BaseData {
    float price;

    static std::string createTable() {
        return "CREATE TABLE IF NOT EXISTS DataElPrice ("
               "epoch INTEGER PRIMARY KEY,"
               "price REAL);";
    }

    std::string insert() const override {
        return "INSERT INTO DataElPrice (epoch, price) VALUES ("
               + std::to_string(epoch) + ","
               + std::to_string(price) + ");";
    }
};

struct DataHistory : public BaseData {
    std::string device;
    int state;

    static std::string createTable() {
        return "CREATE TABLE IF NOT EXISTS DataHistory ("
               "epoch INTEGER PRIMARY KEY,"
               "device TEXT,"
               "state INTEGER);";
    }

    std::string insert() const override {
        return "INSERT INTO DataHistory (epoch, device, state) VALUES ("
               + std::to_string(epoch) + ","
               + "'" + device + "',"
               + std::to_string(state) + ");";
    }
};

class Database {
public:
    Database(const std::string& filename) {
        sqlite3_open(filename.c_str(), &db);

        std::string sql = DataTemperature::createTable() + \
                          DataWeather::createTable() + \
                          DataElPrice::createTable() + \
                          DataHistory::createTable();    
        sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    }

    ~Database() {
        sqlite3_close(db);
    }

    void insert(const DataTemperature& data) {
        sqlite3_exec(db, data.insert().c_str(), 0, 0, 0);
    }

    void insert(const DataWeather& data) {
        sqlite3_exec(db, data.insert().c_str(), 0, 0, 0);
    }

    void insert(const DataElPrice& data) {
        sqlite3_exec(db, data.insert().c_str(), 0, 0, 0);
    }

    DataTemperature getTemperature(long epoch) {
        DataTemperature data;
        std::string sql = "SELECT * FROM DataTemperature WHERE epoch = " + std::to_string(epoch) + ";";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                data.epoch = sqlite3_column_int64(stmt, 0);
                data.temperature = sqlite3_column_double(stmt, 1);
                data.humidity = sqlite3_column_double(stmt, 2);
            }
            sqlite3_finalize(stmt);
        }
        return data;
    }

    DataWeather getWeather(long epoch) {
        DataWeather data;
        std::string sql = "SELECT * FROM DataWeather WHERE epoch = " + std::to_string(epoch) + ";";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                data.epoch = sqlite3_column_int64(stmt, 0);
                data.temperature = sqlite3_column_double(stmt, 1);
                data.humidity = sqlite3_column_double(stmt, 2);
                data.pressure = sqlite3_column_double(stmt, 3);
                data.windSpeed = sqlite3_column_double(stmt, 4);
                data.windDirection = sqlite3_column_double(stmt, 5);
                data.rain = sqlite3_column_double(stmt, 6);
                data.uv = sqlite3_column_double(stmt, 7);
                data.solarRadiation = sqlite3_column_double(stmt, 8);
            }
            sqlite3_finalize(stmt);
        }
        return data;
    }

    DataElPrice getElPrice(long epoch) {
        DataElPrice data;
        std::string sql = "SELECT * FROM DataElPrice WHERE epoch = " + std::to_string(epoch) + ";";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                data.epoch = sqlite3_column_int64(stmt, 0);
                data.price = sqlite3_column_double(stmt, 1);
            }
            sqlite3_finalize(stmt);
        }
        return data;
    }
private:
    sqlite3* db;
};
