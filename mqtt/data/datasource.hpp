#pragma once
#include <string>
#include <variant>
#include <iostream>

enum class DataType {
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
    DataType getType() const {
        if (std::holds_alternative<int>(value)) {
            return DataType::INT;
        } else if (std::holds_alternative<double>(value)) {
            return DataType::DOUBLE;
        } else if (std::holds_alternative<std::string>(value)) {
            return DataType::STRING;
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

    void insert(IDataHeader& header) {
        std::ostringstream out;

        for (auto& value : header) {
            switch (value.getType()) {
                case DataType::INT:
                    out << value.getValue<int>() << " ";
                    break;
                case DataType::DOUBLE:
                    out << value.getValue<double>() << " ";
                    break;
                case DataType::STRING:
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
                case DataType::INT:
                    int i;
                    sstream >> i;
                    value.setValue<int>(i);
                    break;
                case DataType::DOUBLE:
                    double d;
                    sstream >> d;
                    value.setValue<double>(d);
                    break;
                case DataType::STRING:
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


/*
class SourceSqlite : public IDataSource {
    std::string createSql(IDataHeader& header) {
        std::string output = "{";
        for (auto const& value : header) {
            output.append(" ");
            output.append(value.getName());
        }
        output.append(" }");
        return output; 
    }
};
*/
