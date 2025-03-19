#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <vector>
#include "../common/ICommand.hpp"


extern void parse(const char* cache_file);
extern void createWeather(const std::string& filename);

//------------------------------------------------------------------------------
// Helper function to write data received by curl to a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//------------------------------------------------------------------------------
class CreateDatabaseCommand : public ICommand {
 public:
    CreateDatabaseCommand() :
        ICommand("-c", "Create database.")
    {}

    void execute() override {
        spdlog::info("command create database");
        createWeather("data_yaha.db");
    }
};

//------------------------------------------------------------------------------
class DownloadCommand : public ICommand {
public:
    explicit DownloadCommand(std::string cache_file) :
        ICommand("-d", "Download latest weather from network."),
        url("opendata.fmi.fi"),
        path("/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::forecast::harmonie::surface::point::multipointcoverage&place="),
        cache_file(cache_file)
    {
        char *location = getenv("LOCATION");
        if (location == NULL) {
            std::cerr << "ERROR: Missing LOCATION environmental variable!" << std::endl;
            return;
        }
        path.append(location);
    }

    void execute() override {
        spdlog::info("command download (curl): {}", cache_file);

        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            std::string full_url = url + path;
            curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
            } else {
                std::ofstream outfile(cache_file);
                outfile << readBuffer << std::endl;
                outfile.close();
            }
            curl_easy_cleanup(curl);
        }
    }

    /* void httplib_execute() {
        httplib::Client cli(url, 80);

        if (auto res = cli.Get(path.c_str())) {
            if (res->status == httplib::StatusCode::OK_200) {
                std::cout << res->body << std::endl;

                std::ofstream outfile(cache_file);
                outfile << res->body << std::endl;
                outfile.close();
			}
			else {
				std::cout << "HTTP error: " << res->status << std::endl;
			}

        } else {
            auto err = res.error();
            std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
    }*/

private:
    std::string url;
    std::string path;
    std::string cache_file;
};

//------------------------------------------------------------------------------
class ParseCommand : public ICommand {
public:
    explicit ParseCommand(std::string cache_file) :
        ICommand("-p", "Parse weather information from cache file."),
        cache_file(cache_file)
    {}

    void execute() override {
        parse(cache_file.c_str());
    }

private:
    std::string cache_file;
};


//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    std::string cache_file = "tmp_weather.xml";

    std::vector<std::shared_ptr<ICommand>> commands {
        std::make_shared<CreateDatabaseCommand>(),
        std::make_shared<DownloadCommand>(cache_file),
        std::make_shared<ParseCommand>(cache_file)
    };

    if (argc == 2) {
        std::string arg = argv[1];
        for (auto command : commands) {
            if (command->flag() == arg) {
                command->execute();
            }
        }
    } else {
        std::cout << "Usage: weather <command>" << std::endl;
        std::cout << "Commands:" << std::endl;
        for (auto command : commands) {
            std::cout << "  " << command->flag() << " - " << command->description() << std::endl;
        }
    }
    return 0;
}
