#pragma once
#include <sqlite3.h>
#include <string>
#include <functional>
#include <stdexcept>

template <typename T>
class BaseIterator {
public:
    virtual ~BaseIterator() {}

    // Prefix increment
    virtual BaseIterator& operator++() = 0;

    // Dereference
    virtual T operator*() const = 0;

    // Comparison
    virtual bool operator!=(const BaseIterator& other) const = 0;
};

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


// Sqlite iterator
//
template <typename T>
class SqlIterator : public BaseIterator<T> {
public:
    SqlIterator(sqlite3_stmt* stmt, std::function<T(sqlite3_stmt*)> rowToData) :
        stmt(stmt), 
        rowToData(rowToData), 
        done(false)
    {
        step();
    }

    BaseIterator<T>& operator++() override {
        step();
        return *this;
    }

    T operator*() const override {
        return rowToData(stmt);
    }

    bool operator!=(const BaseIterator<T>& other) const override {
        const SqlIterator<T>* otherPtr = dynamic_cast<const SqlIterator<T>*>(&other);
        if (!otherPtr) {
            throw std::runtime_error("Incompatible iterators");
        }
        return done != otherPtr->done;
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

