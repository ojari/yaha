#pragma once
#include <sqlite3.h>
#include <string>
#include <fstream>
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
class SqlIteratorOld : public BaseIterator<T> {
public:
    SqlIteratorOld(sqlite3_stmt* stmt, std::function<T(sqlite3_stmt*)> rowToData) :
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
        const SqlIteratorOld<T>* otherPtr = dynamic_cast<const SqlIteratorOld<T>*>(&other);
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
