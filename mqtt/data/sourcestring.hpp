#pragma once
#include <sstream>
#include "datasource.hpp"

namespace data {

class StringException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

template <typename T>
class SourceString {
public:
    explicit SourceString(const std::string& data) :
        sstream(data)
    {
    }

    bool isEnd() const {
        return sstream.eof();
    }

    void insert(IDataHeader& header) {
        std::ostringstream out;

        for (const auto& value : header) {
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
                default:
                    throw StringException("Unknown data type");
            }
        }
        out << "\n";
        sstream << out.str();
    }

    void read(IDataHeader& header) const {
        std::string s;
        char c = readNonSpace();
        if (c != '{') {
            throw StringException("Expected '{' at the beginning of the data");
        }
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
                    sstream >> s;
                    value.setValue<std::string>(s);
                    break;
                default:
                    throw StringException("Unknown data type");
            }
        }
        c = readNonSpace();
        if (c != '}') {
            throw StringException("Expected '}' at the end of the data");
        }
    }

private:
    // method that reads stream until first non-whitespace character or end of stream.
    // The first non-whitespace character is returned.
    char readNonSpace() const {
        char c;
        while (sstream.get(c) && std::isspace(c)) {}
        return c;
    }

    mutable std::stringstream sstream;
};

} // namespace data