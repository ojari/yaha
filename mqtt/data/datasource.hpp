#pragma once
#include <string>
#include <variant>
#include <iostream>
#include <sqlite3.h>
#include "database.hpp"

enum class DataValueType {
    INT,
    DOUBLE,
    STRING
};

class DataValue {
public:
    DataValue(const std::string& name, int value) : name(name), value(value) {}
    DataValue(const std::string& name, double value) : name(name), value(value) {}
    DataValue(const std::string& name, const std::string& value) : name(name), value(value) {}

    const std::string& getName() const {
        return name;
    }
    DataValueType getType() const {
        if (std::holds_alternative<int>(value)) {
            return DataValueType::INT;
        } else if (std::holds_alternative<double>(value)) {
            return DataValueType::DOUBLE;
        } else if (std::holds_alternative<std::string>(value)) {
            return DataValueType::STRING;
        }
        throw std::runtime_error("Invalid data type");
    }

    template <typename T>
    T getValue() const {
        return std::get<T>(value);
    }

    template <typename T>
    void setValue(T& value) {
        this->value.emplace<T>(value);
    }
private:
    std::string name;
    std::variant<int, double, std::string> value;
};



template <typename T>
struct IDataTable {
    IDataTable(const std::string& tableName) : 
        tableName(tableName)
    {}
    virtual ~IDataTable() {}

    const std::string& getTableName() const {
        return tableName;
    }

    virtual void set(const T& data) = 0;

protected:
    std::string tableName;
};


struct IDataHeader {
    using iterator = std::vector<DataValue>::iterator;

    virtual iterator begin() = 0;
    virtual iterator end() = 0;
};


template <typename T>
class SourceString {
public:
    SourceString(std::string data) : 
        sstream(data)
    {
    }

    bool isEnd() {
        return sstream.eof();
    }

    void insert(IDataHeader& header) {
        std::ostringstream out;

        for (auto& value : header) {
            switch (value.getType()) {
                case DataValueType::INT:
                    out << value.getValue<int>() << " ";
                    break;
                case DataValueType::DOUBLE:
                    out << value.getValue<double>() << " ";
                    break;
                case DataValueType::STRING:
                    out << value.getValue<std::string>() << " ";
                    break;
            }
        }
        out << "\n";
        sstream << out.str();
    }

    void read(IDataHeader& header) const {
        for (auto& value : header) {
            switch (value.getType()) {
                case DataValueType::INT:
                    int i;
                    sstream >> i;
                    value.setValue<int>(i);
                    break;
                case DataValueType::DOUBLE:
                    double d;
                    sstream >> d;
                    value.setValue<double>(d);
                    break;
                case DataValueType::STRING:
                    std::string s;
                    sstream >> s;
                    value.setValue<std::string>(s);
                    break;
            }
        }
    }

private:
    mutable std::stringstream sstream;
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

    void insert(IDataHeader& header) {
        // std::string sql = createSql(header);
    }
    void read(IDataHeader& header) const {
        // std::string sql = createSql(header);
    }
    bool isEnd() const {
        return true;
    }
private:
    sqlite3* db;
};
