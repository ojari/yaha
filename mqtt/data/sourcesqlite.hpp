#pragma once
#include <sqlite3.h>
#include <spdlog/spdlog.h>
#include <functional>
#include "tables.hpp"

namespace data {

class SqlException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class SqlInsert {
public:
    SqlInsert(sqlite3* db, const char* sql) {
        int err = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (err != SQLITE_OK) {
            spdlog::error("Failed to prepare statement: {}", sqlite3_errmsg(db));
        }
    }

    ~SqlInsert() {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    void add(long value) {
        int err = sqlite3_bind_int64(stmt, index, value);
        if (err != SQLITE_OK) {
            spdlog::error("Failed to bind value: {}", sqlite3_errmsg(sqlite3_db_handle(stmt)));
        }
        index++;
    }

    void add(int value) {
        int err = sqlite3_bind_int(stmt, index, value);
        if (err != SQLITE_OK) {
            spdlog::error("Failed to bind value: {}", sqlite3_errmsg(sqlite3_db_handle(stmt)));
        }
        index++;
    }

    void add(float value) {
        int err = sqlite3_bind_double(stmt, index, value);
        if (err != SQLITE_OK) {
            spdlog::error("Failed to bind value: {}", sqlite3_errmsg(sqlite3_db_handle(stmt)));
        }
        index++;
    }

    void add(const std::string& value) {
        int err = sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (err != SQLITE_OK) {
            spdlog::error("Failed to bind value: {}", sqlite3_errmsg(sqlite3_db_handle(stmt)));
        }
        index++;
    }

private:
    sqlite3_stmt* stmt;
    int index = 1;
};

class SqlIterator {
public:
    SqlIterator(sqlite3_stmt* stmt, DataHeader &header) :
        stmt(stmt),
        header(header)
    {
        step();
    }

    SqlIterator& operator++() {
        step();
        return *this;
    }

    DataHeader& operator*() const {
        int index = 1;
        std::string s;
        int i;
        long l;
        float f;
        for (auto & value : header) {
            switch (value.getType()) {
                case DataValueType::INT:
                    i = sqlite3_column_int(stmt, index);
                    value.setValue<int>(i);
                    break;
                case DataValueType::LONG:
                    l = sqlite3_column_int64(stmt, index);
                    value.setValue<long>(l);
                    break;
                case DataValueType::DOUBLE:
                    f = static_cast<float>(sqlite3_column_double(stmt, index));
                    value.setValue<float>(f);
                    break;
                case DataValueType::STRING:
                    s = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
                    value.setValue<std::string>(s);
                    break;
                default:
                    throw SqlException("Unknown data type");
            }
            index++;
        }
        return header;
    }

    bool operator!=(const SqlIterator& other) const {
        const auto* otherPtr = dynamic_cast<const SqlIterator*>(&other);
        if (!otherPtr) {
            throw SqlException("Incompatible iterators");
        }
        return done != otherPtr->done;
    }

private:
    sqlite3_stmt* stmt;
    DataHeader& header;
    bool done = false;

    void step() {
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            done = true;
            sqlite3_finalize(stmt);
        }
    }
};


// @todo: move sql statement creation to cpp file.
class SourceSqlite {
public:
    explicit SourceSqlite(sqlite3* db) :
        db(db)
    {
    }

    std::string createSql(IDataHeader& header) const {
        std::string output = "CREATE TABLE IF NOT EXISTS ";
        output.append(header.getTableName());
        output.append("(");
        for (auto const& value : header) {
            output.append(value.getName());
            output.append(" ");
            switch (value.getType()) {
                case DataValueType::INT:
                case DataValueType::LONG:
                    output.append("INTEGER");
                    break;
                case DataValueType::DOUBLE:
                    output.append("REAL");
                    break;
                case DataValueType::STRING:
                    output.append("TEXT");
                    break;
                default:
                    throw SqlException("Unknown data type");
            }
            output.append(",");
        }
        output.pop_back();  // remove last comma
        output.append(");");
        return output;
    }

    std::string insertSql(IDataHeader& header) const {
        std::string output = "INSERT INTO ";
        output.append(header.getTableName());
        output.append("(");

        for (auto const& value : header) {
            output.append(value.getName());
            output.append(",");
        }
        output.pop_back();  // remove last comma

        output.append(" ) VALUES (");
        for (auto const& value : header) {
            output.append("?,");
        }
        output.pop_back();  // remove last comma
        output.append(");");
        return output;
    }

    std::string selectSql(IDataHeader& header) const {
        std::string output = "SELECT ";

        for (auto const& value : header) {
            output.append(value.getName());
            output.append(",");
        }
        output.pop_back();  // remove last comma

        output.append(" FROM ");
        output.append(header.getTableName());
        output.append(";");
        return output;
    }

    SqlIterator begin(DataHeader& header) {
        std::string sql = selectSql(header);
        sqlite3_stmt* stmt;
        int err = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (err != SQLITE_OK) {
            spdlog::error("Failed to prepare statement: {}", sqlite3_errmsg(db));
        }
        return SqlIterator(stmt, header);
    }

    void createTable(IDataHeader& header) {
        std::string sqlStr = createSql(header);
        int err = sqlite3_exec(db, sqlStr.c_str(), nullptr, nullptr, nullptr);
        if (err != SQLITE_OK) {
            spdlog::error("Failed to create table: {}", sqlite3_errmsg(db));
            // throw SqlException("Failed to create table");
        }
    }

    void insert(IDataHeader& header) {
        std::string sqlStr = insertSql(header);
        auto sql = SqlInsert(db, sqlStr.c_str());
        for (auto const& value : header) {
            switch (value.getType()) {
                case DataValueType::INT:
                    sql.add(value.getValue<int>());
                    break;
                case DataValueType::LONG:
                    sql.add(value.getValue<long>());
                    break;
                case DataValueType::DOUBLE:
                    sql.add(value.getValue<float>());
                    break;
                case DataValueType::STRING:
                    sql.add(value.getValue<std::string>());
                    break;
                default:
                    throw SqlException("Unknown data type");
            }
        }
    }

    bool isEnd() const {
        return true;
    }

private:
    sqlite3* db;
};

extern sqlite3* createDatabase(const std::string& filename);

}  // namespace data
