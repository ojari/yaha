#include <array>
#include <charconv>
#include <cctype>
#include <iostream>
#include <optional>
#include <span>
#include <string_view>
#include <system_error>
#include <vector>
#include "weather_parse.hpp"

#include <tinyxml2.h>

#include "../common/utils.hpp"
#include "../mqtt/data/data.hpp"

using namespace tinyxml2;
using namespace std;

using DataWeather = data::DataWeather;

namespace {

constexpr array<string_view, 7> path_position = {
    string_view{"wfs:member"},
    string_view{"omso:GridSeriesObservation"},
    string_view{"om:result"},
    string_view{"gmlcov:MultiPointCoverage"},
    string_view{"gml:domainSet"},
    string_view{"gmlcov:SimpleMultiPoint"},
    string_view{"gmlcov:positions"}
};

constexpr array<string_view, 7> path_data = {
    string_view{"wfs:member"},
    string_view{"omso:GridSeriesObservation"},
    string_view{"om:result"},
    string_view{"gmlcov:MultiPointCoverage"},
    string_view{"gml:rangeSet"},
    string_view{"gml:DataBlock"},
    string_view{"gml:doubleOrNilReasonTupleList"}
};

inline void skipWhitespace(string_view& buffer)
{
    const auto first = buffer.find_first_not_of(" \t\r\n");
    if (first == string_view::npos) {
        buffer = {};
        return;
    }
    buffer.remove_prefix(first);
}

template <typename T>
bool readNumber(string_view& buffer, T& value)
{
    skipWhitespace(buffer);
    if (buffer.empty()) {
        return false;
    }

    const auto begin = buffer.data();
    const auto end = begin + buffer.size();
    const auto result = from_chars(begin, end, value);
    if (result.ec != errc()) {
        return false;
    }

    const auto consumed = result.ptr - begin;
    buffer.remove_prefix(consumed);
    return true;
}

template <typename F>
void forEachValueLine(const char* text, F&& handler)
{
    if (text == nullptr) {
        return;
    }

    string_view buffer{text};
    while (!buffer.empty()) {
        const auto newline = buffer.find('\n');
        string_view line = (newline == string_view::npos)
            ? buffer
            : string_view{buffer.data(), newline};

        while (!line.empty() && line.back() == '\r') {
            line.remove_suffix(1);
        }

        if (line.size() > 20) {
            handler(line);
        }

        if (newline == string_view::npos) {
            break;
        }
        buffer.remove_prefix(newline + 1);
    }
}

optional<XMLElement*> findElement(XMLElement* root, span<const string_view> tags)
{
    XMLElement* node = root;
    for (const auto& tag : tags) {
        node = node->FirstChildElement(tag.data());
        if (node == nullptr) {
            cout << "Can't find " << tag << " from weather data!" << endl;
            return nullopt;
        }
    }
    return node;
}

} // namespace

//------------------------------------------------------------------------------
void WeatherParse::parseEpoch(const char* text)
{
    forEachValueLine(text, [this](string_view line) {
        [[maybe_unused]] double latitude;
        [[maybe_unused]] double longitude;
        long epoch = 0;

        if (!readNumber(line, latitude) || !readNumber(line, longitude) || !readNumber(line, epoch)) {
            return;
        }

        data.emplace_back(epoch);
    });
}

//------------------------------------------------------------------------------
void WeatherParse::parseWeather(const char* text)
{
    auto it = data.begin();
    forEachValueLine(text, [this, &it](string_view line) {
        if (it == data.end()) {
            return;
        }

        array<float, 15> values;
        for (auto& value : values) {
            if (!readNumber(line, value)) {
                return;
            }
        }

        it->setPressure(values[0]);
        it->setTemperature(values[2]);
        it->setHumidity(values[4]);
        it->setWindDirection(values[5]);
        it->setWindSpeed(values[6]);
        it->setRain(values[9]);
        it->setSolarRadiation(values[14]);

        ++it;
    });
}

//------------------------------------------------------------------------------
void WeatherParse::print()
{
    for (const auto& item : data) {
        print_epoch(item.epoch);
        cout << " "
             << item.temperature << " "
             << item.humidity << " "
             << item.pressure << " "
             << item.windSpeed << " "
             << item.windDirection << " "
             << item.rain << " "
             << item.solarRadiation << endl;
    }
}

//------------------------------------------------------------------------------
void WeatherParse::execute()
{
    XMLDocument doc;

    if (doc.LoadFile(cache_file.c_str()) != XML_SUCCESS) {
        cout << "Can't read weather xml file!" << endl;
        return;
    }

    if (auto root = doc.RootElement(); root != nullptr) {
        if (auto node = findElement(root, path_position); node && (*node)->GetText() != nullptr) {
            parseEpoch((*node)->GetText());
        } else {
            return;
        }

        if (auto node = findElement(root, path_data); node && (*node)->GetText() != nullptr) {
            parseWeather((*node)->GetText());
        } else {
            return;
        }

        print();
    } else {
        cout << "Weather XML does not contain a root element!" << endl;
    }
}
