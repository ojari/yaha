#pragma once
#include "datasource.hpp"
#include <sstream>

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
