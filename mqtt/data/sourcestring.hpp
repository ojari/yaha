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
                case DataValueType::LONG:
                    out << value.getValue<long>() << " ";
                    break;
                case DataValueType::DOUBLE:
                    out << value.getValue<float>() << " ";
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
                case DataValueType::LONG:
                    long li;
                    sstream >> li;
                    value.setValue<long>(li);
                    break;
                case DataValueType::DOUBLE:
                    float d;
                    sstream >> d;
                    value.setValue<float>(d);
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
