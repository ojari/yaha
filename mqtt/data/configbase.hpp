#pragma once
#include <sqlite3.h>
#include "config.hpp"
#include "common.hpp"
#include <memory>

class ConfigDeviceORM : public BaseORM {
public:
    ConfigDeviceORM(sqlite3* db) :
        BaseORM(db) 
    {}

    const char* sqlCreateTable() override {
        return "CREATE TABLE IF NOT EXISTS Device ("
               "name TEXT PRIMARY KEY,"
               "type TEXT);";
    }

    void insert(const ConfigDevice& data) {
        auto sql = SqlInsert(db, "INSERT INTO Device (name, type) VALUES (?, ?);");
        sql.add(data.name);
        sql.add(data.type);
    }

    SqlIterator<ConfigDevice> begin() {
        const char* sql = "SELECT name, type FROM Device;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        return SqlIterator<ConfigDevice>(stmt, [](sqlite3_stmt* stmt) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            return ConfigDevice(name, type);
        });
    }

    SqlIterator<ConfigDevice> end() {
        return SqlIterator<ConfigDevice>(nullptr, {});
    }
};

class ConfigControllerORM : public BaseORM {
public:
    ConfigControllerORM(sqlite3* db) :
        BaseORM(db) 
    {}

    const char* sqlCreateTable() override {
        return "CREATE TABLE IF NOT EXISTS Controller ("
               "name TEXT PRIMARY KEY,"
               "type TEXT,"
               "actuator TEXT,"
               "time1 INTEGER,"
               "time2 INTEGER);";
    }

    void insert(const ConfigController& data) {
        auto sql = SqlInsert(db, "INSERT INTO Controller (name, type, actuator, time1, time2) VALUES (?, ?, ?, ?, ?);");
        sql.add(data.name);
        sql.add(data.type);
        sql.add(data.actuator);
        sql.add(data.time1);
        sql.add(data.time2);
    }

    SqlIterator<ConfigController> begin() {
        const char* sql = "SELECT name, type, actuator, time1, time2 FROM Controller;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        return SqlIterator<ConfigController>(stmt, [](sqlite3_stmt* stmt) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string actuator = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int time1 = sqlite3_column_int(stmt, 3);
            int time2 = sqlite3_column_int(stmt, 4);
            return ConfigController(name, type, actuator, time1, time2);
        });
    }

    SqlIterator<ConfigController> end() {
        return SqlIterator<ConfigController>(nullptr, {});
    }
};

class ConfigDatabase {
public:
    ConfigDatabase(sqlite3* db) :
        device(db),
        controller(db),
        db(db)
    {
    }

    ~ConfigDatabase() {
        sqlite3_close(db);
    }

    void insert(const ConfigDevice& data) {
        device.insert(data);
    }

    void insert(const ConfigController& data) {
        controller.insert(data);
    }


    ConfigDeviceORM device;
    ConfigControllerORM controller;

private:
    sqlite3* db;
};