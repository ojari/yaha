#pragma once
#include <fstream>
#include "config.hpp"
#include "common.hpp"
#include <memory>

template <typename T>
class FileIterator : public BaseIterator<T> {
public:
    FileIterator(std::ifstream& file, std::function<T(const std::string&)> rowToData) :
        file(file), 
        rowToData(rowToData)
    {
        step();
    }

    BaseIterator<T>& operator++() override {
        step();
        return *this;
    }

    T operator*() const override {
        return rowToData(line);
    }

    bool operator!=(const BaseIterator<T>& other) const override {
        const FileIterator<T>* otherPtr = dynamic_cast<const FileIterator<T>*>(&other);
        if (!otherPtr) {
            throw std::runtime_error("Incompatible iterators");
        }
        return !file.eof();
    }
private:
    std::ifstream& file;
    std::function<T(const std::string&)> rowToData;
    std::string line;

    void step() {
        std::getline(file, line);
    }
};


struct ConfigDevice {
    std::string name;
    std::string type;

    ConfigDevice(const std::string& name, const std::string& type) :
        name(name),
        type(type)
    {}
};


class ConfigDeviceTxt : public BaseORM<ConfigDevice> {
public:
    ConfigDeviceTxt(const char* filename) :
        BaseORM(),
        filename(filename)
    {
    }

    const char* sqlCreateTable() override {
        return "";
    }

    void insert(const ConfigDevice& data) {
        throw std::runtime_error("Not implemented");
    }

    BaseIterator<ConfigDevice>* begin() override {
        return new FileIterator<ConfigDevice>(file, [](const std::string& line) {
            std::istringstream iss(line);
            std::string start, name, type, end;
            iss >> start >> name >> type >> end;
            return ConfigDevice(name, type);
        });
    }
};