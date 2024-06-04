#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <CLI/CLI.hpp>
#include "../common/utils.hpp"
#include "../common/ICommand.hpp"

using namespace std;

extern void parse(const char* cache_file);


//------------------------------------------------------------------------------
class DownloadCommand : public ICommand {
public:
    DownloadCommand(string cache_file) :
        ICommand("-d", "Download latest weather from network."),
        url("http://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::forecast::harmonie::surface::point::multipointcoverage&place="),
        cache_file(cache_file)
    {
        char *location = getenv("LOCATION");
        if (location == NULL) {
            cerr << "ERROR: Missing LOCATION environmental variable!" << std::endl;
            return;
        }
        url.append(location);
    }

    void execute() override {
        cout << "command download: " << cache_file << endl;
        download(url.c_str(), cache_file.c_str());
    }

private:
    string url;
    string cache_file;
};

//------------------------------------------------------------------------------
class ParseCommand : public ICommand {
public:
    ParseCommand(string cache_file) :
        ICommand("-p", "Parse weather information from cache file."),
        cache_file(cache_file)
    {}

    void execute() override {
        parse(cache_file.c_str());
    }

private:
    string cache_file;
};

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    CLI::App app;
    string cache_file = "tmp_weather.xml";

    vector<shared_ptr<ICommand>> commands {
        make_shared<DownloadCommand>(cache_file),
        make_shared<ParseCommand>(cache_file)
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
