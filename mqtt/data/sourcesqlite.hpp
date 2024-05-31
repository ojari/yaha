#pragma once
#include <sqlite3.h>
#include "datasource.hpp"

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
                    output.append("INTEGER");
                    break;
                case DataValueType::DOUBLE:
                    output.append("DOUBLE");
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

    void insert(IDataHeader& header) {
        // std::string sql = insertSql(header);
    }

    bool isEnd() const {
        return true;
    }
private:
    sqlite3* db;
};

#include "sourcesqlite.cpp"