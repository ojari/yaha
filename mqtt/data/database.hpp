#pragma once
#include <sqlite3.h>
#include "data.hpp"
#include "common.hpp"
#include <vector>
#include <memory>


class DataElPriceORM : public BaseORM<DataElPrice> {
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

    BaseIterator<DataElPrice>* begin() override {
        const char* sql = "SELECT epoch, price FROM ElPrice;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        return new SqlIterator<DataElPrice>(stmt, [](sqlite3_stmt* stmt) {
            long epoch = sqlite3_column_int64(stmt, 0);
            float price = sqlite3_column_double(stmt, 1);
            return DataElPrice(epoch, price);
        });
    }

    BaseIterator<DataElPrice>* end() override {
        return new SqlIterator<DataElPrice>(nullptr, {});
    }
};

class DataHistoryORM : public BaseORM<DataHistory>, public DataInsertHistory {
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

    BaseIterator<DataHistory>* begin() override {
        const char* sql = "SELECT epoch, device, type, val1, val2, val3 FROM History;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        return new SqlIterator<DataHistory>(stmt, [](sqlite3_stmt* stmt) {
            long epoch = sqlite3_column_int64(stmt, 0);
            std::string device = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            DataType type = static_cast<DataType>(sqlite3_column_int(stmt, 2));
            int val1 = sqlite3_column_int(stmt, 3);
            int val2 = sqlite3_column_int(stmt, 4);
            int val3 = sqlite3_column_int(stmt, 5);
            return DataHistory(epoch, device, type, val1, val2, val3);
        });
    }

    BaseIterator<DataHistory>* end() override {
        return new SqlIterator<DataHistory>(nullptr, {});
    }
};


class Database {
public:
    Database(sqlite3* db) : 
        history(db),
        db(db)
    {
    }

    ~Database() {
        sqlite3_close(db);
    }

    void createTables() {
        createTable(history.sqlCreateTable());
    }
    
    void insert(const DataHistory& data) {
        history.insert(data);
    }

    DataHistoryORM history;

private:
    void createTable(const char* sql);

    sqlite3* db;
};


std::unique_ptr<Database> createDatabase(const std::string& filename);
