#include <tinyxml2.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "../common/utils.hpp"

using namespace tinyxml2;
using namespace std;

struct WeatherData {
    long epoch;
    double temperature;
    double humidity;
    double pressure;
    double geopHeight;
    double dewPoint;
};

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
void parseEpoch(string buffer, vector<WeatherData>& data)
{
    istringstream sstream(buffer);
    string line;

    while (getline(sstream, line)) {
        if (line.length() > 20) {
            istringstream linestream(line);
            double latitude, longitude;
            long epoch;
            
            linestream >> latitude >> longitude >> epoch;

            data.push_back(WeatherData {epoch, 0, 0, 0, 0, 0});
        }
    }
}

void parseWeather(string buffer, vector<WeatherData>& data)
{
    istringstream sstream(buffer);
    string line;
    int counter = 0;

    while (getline(sstream, line)) {
        if (line.length() > 20) {
            istringstream linestream(line);
            linestream >> data[counter].pressure 
                       >> data[counter].geopHeight
                       >> data[counter].temperature
                       >> data[counter].dewPoint
                       >> data[counter].humidity;
            counter++;
        }
    }
}

void printWeatherData(vector<WeatherData>& data)
{
    for (auto& item : data) {
        print_epoch(item.epoch);
        cout << " "
             << item.temperature << " "
             << item.humidity << " "
             << item.pressure << " "
             << item.geopHeight << " "
             << item.dewPoint << endl;
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

    vector<WeatherData> data;

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
}

