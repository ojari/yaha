#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <iostream>
#include <fstream>
#include "ICommand.hpp"

class SpotDownload : public ICommand {
public:
    explicit SpotDownload(const std::string& cache_file) :
        ICommand("-sd", "Download latest spot prices from network."),
        cache_file(cache_file)
    {
    }

    virtual ~SpotDownload() = default;

    void execute() override {
        std::string url { "api.spot-hinta.fi" };
        std::string path { "/Today?region=fi&priceResolution=15" };
        httplib::SSLClient cli(url);
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
