#include <tinyxml2.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "../common/utils.hpp"
#include "data/data.hpp"
#include "data/tables.hpp"
#include "data/sourcesqlite.hpp"

using namespace tinyxml2;
using namespace std;


//------------------------------------------------------------------------------
void print_elements(XMLElement *element)
{
    for (XMLElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        cout << " - " << child->Value() << endl;
    }
}

//------------------------------------------------------------------------------
XMLElement *set_location(XMLElement *root, vector<string>& path)
{
    XMLElement *node = root;

    for(auto tagName : path) {
        node = node->FirstChildElement(tagName.c_str());
        if (node == NULL) {
            cout << "Can't find " << tagName << " from weather data!" << endl;
            break;
        }
    }
    return node;
}

//------------------------------------------------------------------------------
void parseEpoch(string buffer, vector<DataWeather>& data)
{
    istringstream sstream(buffer);
    string line;

    while (getline(sstream, line)) {
        if (line.length() > 20) {
            istringstream linestream(line);
            double latitude, longitude;
            long epoch;
            
            linestream >> latitude >> longitude >> epoch;

            data.push_back(DataWeather {epoch});
        }
    }
}

//------------------------------------------------------------------------------
void parseWeather(string buffer, vector<DataWeather>& data)
{
    istringstream sstream(buffer);
    string line;
    int counter = 0;

    while (getline(sstream, line)) {
        if (line.length() > 20) {
            istringstream linestream(line);
            float pressure, temperature, humidity, windDirection, windSpeed, rain, solarRadiation;
            float geopHeight,dewPoint, windUms, windVms, totalCloudCover, lowCloudCover, mediumCloudCover, highCloudCover;
            linestream >> pressure
                       >> geopHeight
                       >> temperature
                       >> dewPoint
                       >> humidity
                       >> windDirection
                       >> windSpeed
                       >> windUms
                       >> windVms
                       >> rain
                       >> totalCloudCover >> lowCloudCover >> mediumCloudCover >> highCloudCover >> solarRadiation;
            
            data[counter].setTemperature(temperature);
            data[counter].setHumidity(humidity);
            data[counter].setPressure(pressure);
            data[counter].setWindSpeed(windSpeed);
            data[counter].setWindDirection(windDirection);
            data[counter].setRain(rain);
            data[counter].setSolarRadiation(solarRadiation);

            counter++;
        }
    }
}

//------------------------------------------------------------------------------
void printWeatherData(vector<DataWeather>& data)
{
    for (auto& item : data) {
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
void toDatabase(vector<DataWeather>& data)
{
    sqlite3* db = createDatabase("data_yaha.db");
    SourceSqlite source(db);
    TableWeather table;

    for (auto& item : data) {
        cout << "Inserting to database: " << item.epoch << endl;
    
        dataToHeader(table, item);

        source.insert(table);
    }
}

//------------------------------------------------------------------------------
void parse(const char* cache_file)
{
    XMLDocument doc;
    vector<string> path_position = {
        "wfs:member",
        "omso:GridSeriesObservation",
        "om:result",
        "gmlcov:MultiPointCoverage",
        "gml:domainSet",
        "gmlcov:SimpleMultiPoint",
        "gmlcov:positions"
    };

    vector<string> path_data = {
        "wfs:member",
        "omso:GridSeriesObservation",
        "om:result",
        "gmlcov:MultiPointCoverage",
        "gml:rangeSet",
        "gml:DataBlock",
        "gml:doubleOrNilReasonTupleList"
    };

    vector<DataWeather> data;

    if (doc.LoadFile(cache_file) != XML_SUCCESS) {
        cout << "Can't read weather xml file!" << endl;
        return;
    }

    auto *root = doc.RootElement();
    if (root != NULL) {
        XMLElement *node = set_location(root, path_position);

        parseEpoch(node->GetText(), data);
    }
    if (root != NULL) {
        XMLElement *node = set_location(root, path_data);

        parseWeather(node->GetText(), data);
    }
    printWeatherData(data);

    toDatabase(data);
}

