#pragma once
#include <sqlite3.h>
#include "data.hpp"
#include "common.hpp"
#include <vector>
#include <memory>

template <typename T>
class BaseORM {
public:
    BaseORM<T>(sqlite3* db) : db(db) {
    }

    virtual ~BaseORM() {
    }

    virtual const char* sqlCreateTable() = 0;
    virtual void insert(const T& data) = 0;
    virtual BaseIterator<T>* begin() = 0;
    virtual BaseIterator<T>* end() = 0;

protected:
    sqlite3* db;
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
        return new SqlIteratorOld<DataHistory>(stmt, [](sqlite3_stmt* stmt) {
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
        return new SqlIteratorOld<DataHistory>(nullptr, {});
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
