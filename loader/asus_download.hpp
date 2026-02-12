#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <iostream>
#include <fstream>
#include "ICommand.hpp"

class AsusDownload : public ICommand {
public:
    explicit AsusDownload(const std::string& cache_file) :
        ICommand("-ad", "Download latest connected devices from ASUS router."),
        cache_file(cache_file)
    {

    }
    void execute() override {
        std::string url;
        std::string path { "/main.html" };

        char *host = getenv("ASUS_HOST");
        if (host == NULL) {
            std::cerr << "ERROR: Missing ASUS_HOST environmental variable!" << std::endl;
            return;
        }
        url = host;

        httplib::Client cli(url);
        httplib::Headers headers {
            { "User-Agent", "asusrouter-Android-DUTUtil-1.0.0.245" },
        };   

        // cli.enable_server_certificate_verification(true);

        if (auto res = cli.Get(path.c_str(), headers)) {
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
