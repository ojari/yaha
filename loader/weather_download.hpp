#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <iostream>
#include <fstream>
#include "ICommand.hpp"

class WeatherDownload : public ICommand {
public:
    explicit WeatherDownload(const std::string& cache_file) :
        ICommand("-wd", "Download latest weather forecast from network."),
        cache_file(cache_file)
    {

    }
    void execute() override {
        std::string url { "opendata.fmi.fi" };
        std::string path { "/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::forecast::harmonie::surface::point::multipointcoverage&place=" };
        httplib::SSLClient cli(url);

        char *location = getenv("LOCATION");
        if (location == NULL) {
            std::cerr << "ERROR: Missing LOCATION environmental variable!" << std::endl;
            return;
        }
        path.append(location);

        cli.enable_server_certificate_verification(true);

        if (auto res = cli.Get(path.c_str())) {
            if (res->status == httplib::StatusCode::OK_200) {
                std::ofstream outfile(cache_file);
                outfile << res->body << std::endl;
                outfile.close();
            }
            else {
                std::cout << "HTTP status code " << res->status << " - " << res->reason << std::endl;
            }
        } else {
            auto err = res.error();
            std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
    }

private:
    std::string cache_file;
};
