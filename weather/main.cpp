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
struct ICommand {
    virtual void execute() = 0;
    virtual string description() = 0;
    virtual string flag() = 0;

    bool active = false;
};

//------------------------------------------------------------------------------
class DownloadCommand : public ICommand {
public:
    DownloadCommand(string url, string cache_file) : 
        url(url),
        cache_file(cache_file)
    {}
    void execute() override {
        cout << "command download: " << url << " -> " << cache_file << endl;
        download(url.c_str(), cache_file.c_str());
    }
    string description() override {
        return "Download latest weather from network.";
    }
    string flag() override {
        return "-d";
    }
private:
    string url;
    string cache_file;
};

//------------------------------------------------------------------------------
class ParseCommand : public ICommand {
public:
    ParseCommand(string cache_file) :
        cache_file(cache_file)
    {}
    void execute() override {
        cout << "command parse: " << cache_file << endl;
        parse(cache_file.c_str());
    }
    string description() override {
        return "Get weather information from json cache file.";
    }
    string flag() override {
        return "-p";
    }
private:
    string cache_file;
};

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
        cerr << "ERROR: Missing LOCATION environmental variable!" << std::endl;
        return 1;
    }
    url.append(location);

    vector<ICommand*> commands = {
        new DownloadCommand(url, cache_file),
        new ParseCommand(cache_file)
    };

    for (auto command : commands) {
        app.add_flag(command->flag(), command->active, command->description());
    }

    CLI11_PARSE(app, argc, argv);

    for (auto command : commands) {
        if (command->active) {
            command->execute();
        }
    }
    return 0;
}
