#pragma once
#include <sqlite3.h>
#include <string>
#include <functional>

class BaseORM {
public:
    BaseORM(sqlite3* db) : db(db) {
    }

    virtual ~BaseORM() {
    }

    virtual const char* sqlCreateTable() = 0;

protected:
    sqlite3* db;
};

class SqlInsert {
public:
    SqlInsert(sqlite3* db, const char* sql) {
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    }

    ~SqlInsert() {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    void add(long value) {
        sqlite3_bind_int64(stmt, index, value);
        index++;
    }

    void add(int value) {
        sqlite3_bind_int(stmt, index, value);
        index++;
    }

    void add(float value) {
        sqlite3_bind_double(stmt, index, value);
        index++;
    }

    void add(const std::string& value) {
        sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
        index++;
    }
private:
    sqlite3_stmt* stmt;
    int index = 1;
};

template <typename T>
class SqlIterator {
public:
    SqlIterator(sqlite3_stmt* stmt, std::function<T(sqlite3_stmt*)> rowToData) :
        stmt(stmt), 
        rowToData(rowToData), 
        done(false)
    {
        step();
    }

    SqlIterator& operator++() {
        step();
        return *this;
    }

    T operator*() {
        return rowToData(stmt);
    }

    bool operator!=(const SqlIterator& other) const {
        return done != other.done;
    }

private:
    sqlite3_stmt* stmt;
    std::function<T(sqlite3_stmt*)> rowToData;
    bool done;

    void step() {
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            done = true;
            sqlite3_finalize(stmt);
        }
    }
};

