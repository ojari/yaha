#pragma once
#include <sqlite3.h>
#include "datasource.hpp"

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
class SourceSqlite {
public:
    SourceSqlite(sqlite3* db) : 
        db(db)
    {
    }

    std::string createSql(IDataHeader& header, IDataTable<T>& table) {
        std::string output = "CREATE TABLE IF NOT EXISTS ";
        output.append(table.getTableName());
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
            }
            output.append(",");
        }
        output.pop_back(); // remove last comma
        output.append(");");
        return output; 
    }

    std::string insertSql(IDataHeader& header, IDataTable<T>& table) {
        std::string output = "INSERT INFO ";
        output.append(table.getTableName());
        output.append("(");

        for (auto const& value : header) {
            output.append(value.getName());
            output.append(",");
        }
        output.pop_back(); // remove last comma

        output.append(" ) VALUES (");
        for (auto const& value : header) {
            output.append("?,");
        }
        output.pop_back(); // remove last comma
        output.append(");");
        return output;
    }

    void read(IDataHeader& header) const {
        // std::string sql = createSql(header);
    }

    void insert(IDataHeader& header, IDataTable<T>& table) {
        std::string sqlStr = insertSql(header, table);
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
            }
        }
    }

    bool isEnd() const {
        return true;
    }
private:
    sqlite3* db;
};

#include "sourcesqlite.cpp"