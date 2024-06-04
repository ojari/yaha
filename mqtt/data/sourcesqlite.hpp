#pragma once
#include <sqlite3.h>
#include <functional>
#include "common.hpp"
#include "datasource.hpp"

class SqlIterator {
public:
    SqlIterator(sqlite3_stmt* stmt, IDataHeader &header) :
        stmt(stmt),
        header(header),
        done(false)
    {
        step();
    }

    SqlIterator& operator++() {
        step();
        return *this;
    }

    IDataHeader& operator*() const {
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
                    f = sqlite3_column_double(stmt, index);
                    value.setValue<float>(f);
                    break;
                case DataValueType::STRING:
                    s = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
                    value.setValue<std::string>(s);
                    break;
                default:
                    throw std::runtime_error("Unknown data type");
            }
            index++;
        }
        return header;
    }

    bool operator!=(const SqlIterator& other) const {
        const SqlIterator* otherPtr = dynamic_cast<const SqlIterator*>(&other);
        if (!otherPtr) {
            throw std::runtime_error("Incompatible iterators");
        }
        return done != otherPtr->done;
    }

private:
    sqlite3_stmt* stmt;
    IDataHeader& header;
    bool done;

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
    SourceSqlite(sqlite3* db) : 
        db(db)
    {
    }

    std::string createSql(IDataHeader& header) {
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
                    throw std::runtime_error("Unknown data type");
            }
            output.append(",");
        }
        output.pop_back(); // remove last comma
        output.append(");");
        return output; 
    }

    std::string insertSql(IDataHeader& header) {
        std::string output = "INSERT INFO ";
        output.append(header.getTableName());
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

    std::string selectSql(IDataHeader& header) {
        std::string output = "SELECT ";

        for (auto const& value : header) {
            output.append(value.getName());
            output.append(",");
        }
        output.pop_back(); // remove last comma

        output.append(" FROM ");
        output.append(header.getTableName());
        output.append(";");
        return output;
    }

    void read(IDataHeader& header) const {
        // std::string sql = createSql(header);
    }

    SqlIterator* begin(IDataHeader& header) {
        std::string sql = selectSql(header);
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        return new SqlIterator(stmt, header);
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
                    throw std::runtime_error("Unknown data type");
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