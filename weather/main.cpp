#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <httplib.h>
#include "../common/ICommand.hpp"

using namespace std;

extern void parse(const char* cache_file);


//------------------------------------------------------------------------------
class DownloadCommand : public ICommand {
public:
    DownloadCommand(string cache_file) :
        ICommand("-d", "Download latest weather from network."),
        url("opendata.fmi.fi"),
        path("/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::forecast::harmonie::surface::point::multipointcoverage&place="),
        cache_file(cache_file)
    {
        char *location = getenv("LOCATION");
        if (location == NULL) {
            cerr << "ERROR: Missing LOCATION environmental variable!" << std::endl;
            return;
        }
        path.append(location);
    }

    void execute() override {
        cout << "command download: " << cache_file << endl;

        httplib::Client cli(url, 80);

        if (auto res = cli.Get(path.c_str())) {
            if (res->status == httplib::StatusCode::OK_200) {
                // std::cout << res->body << std::endl;

                std::ofstream outfile(cache_file);
                outfile << res->body << std::endl;
                outfile.close();
            }
        } else {
            auto err = res.error();
            std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
        // download(url.c_str(), cache_file.c_str());
    }

private:
    string url;
    string path;
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
    string cache_file = "tmp_weather.xml";

    vector<shared_ptr<ICommand>> commands {
        make_shared<DownloadCommand>(cache_file),
        make_shared<ParseCommand>(cache_file)
    };

    if (argc == 2) {
        string arg = argv[1];
        for (auto command : commands) {
            if (command->flag() == arg) {
                command->execute();
            }
        }
    }
    return 0;
}
