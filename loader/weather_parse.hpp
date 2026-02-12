#pragma once
#include "ICommand.hpp"
#include <string>
#include <vector>
#include "../mqtt/data/data.hpp"

class WeatherParse : public ICommand {
public:
    explicit WeatherParse(std::string cache_file) :
        ICommand("-wp", "Parse weather information from cache file."),
        cache_file(cache_file)
    {}

    virtual ~WeatherParse() = default;

    void execute() override;

    void print();
private:
    void parseEpoch(const char* text);
    void parseWeather(const char* text);

    std::string cache_file;
    std::vector<data::DataWeather> data;

};
