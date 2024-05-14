#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <tinyxml2.h>
#include <CLI/CLI.hpp>
#include "../common/utils.hpp"

using namespace std;
using namespace tinyxml2;


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

    vector<long> times;

    if (doc.LoadFile(cache_file) != XML_SUCCESS) {
        cout << "Can't read weather xml file!" << endl;
        return;
    }

    auto *root = doc.RootElement();
    if (root != NULL) {
        XMLElement *node = set_location(root, path_position);

        istringstream sstream(node->GetText());
        string line;

        while (getline(sstream, line)) {
            if (line.length() > 20) {
                istringstream linestream(line);
                double latitude, longitude;
                long epoch;
                
                linestream >> latitude >> longitude >> epoch;

                times.push_back(epoch);
            }
        }
    }
    if (root != NULL) {
        XMLElement *node = set_location(root, path_data);

        istringstream sstream(node->GetText());
        string line;
        int counter = 0;

        while (getline(sstream, line)) {
            if (line.length() > 20) {
                istringstream linestream(line);
                double pressure, geopHeight, temperature, dewPoint, humidity;

                linestream >> pressure >> geopHeight >> temperature >> dewPoint >> humidity;

                bool last = print_epoch(times[counter]);
                cout << " "
                     << temperature << "C "
                     << humidity << "%" << endl;
                counter++;
                if (last) {
                    cout << "----------------------------------------------------" << endl;
                }
            }
        }
    }    
}

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    CLI::App app;
    bool do_download { false };
    bool do_parse { false };
    string cache_file = "tmp_weather.xml";
    string url { "http://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::forecast::harmonie::surface::point::multipointcoverage&place=" };
    char c;

    char *location = getenv("LOCATION");
    if (location == NULL) {
        std::cerr << "ERROR: Missing LOCATION environmental variable!" << std::endl;
        return 1;
    }
    url.append(location);

    app.add_flag("-d", do_download, "Download latest weather from network.");
    app.add_flag("-p", do_parse, "Get weather information from json cache file.");

    CLI11_PARSE(app, argc, argv);

    if (do_download) {
        download(url.c_str(), cache_file.c_str());
    }
    if (do_parse) {
        parse(cache_file.c_str());
    }
    if (!do_download && !do_parse) {
        std::cout << "Command line argument missing!" << std::endl;
        download(url.c_str(), cache_file.c_str());
        parse(cache_file.c_str());
    }
    return 0;
}
